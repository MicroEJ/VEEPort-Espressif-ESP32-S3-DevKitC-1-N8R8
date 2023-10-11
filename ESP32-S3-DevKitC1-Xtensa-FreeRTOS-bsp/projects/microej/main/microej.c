/*
 * C
 *
 * Copyright 2022-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "spi_flash_mmap.h"
#include "esp_flash.h"
#include "esp_task_wdt.h"
#include "nvs_flash.h"
#include "microej_main.h"
#include "esp_ota_ops.h"
#include "esp_system.h"
#include "sdkconfig.h"
#include "esp_chip_info.h"


#ifdef VALIDATION_BUILD
    #include "t_core_main.h"
#else
    #include "sni.h"
    #include "uart_switch.h"
#endif

#if CONFIG_APPTRACE_SV_ENABLE
    #include "SEGGER_SYSVIEW.h"
#endif // CONFIG_APPTRACE_SV_ENABLE


/******************************************************
 *                    Constants
 ******************************************************/
#define MICROJVM_STACK_SIZE                (5 * 1024)

#ifdef VALIDATION_BUILD
    /* Validation task constants */
    #define VALIDATION_TASK_PRIORITY           ( 8 ) /** Should be > tskIDLE_PRIORITY & < configTIMER_TASK_PRIORITY */
    #define VALIDATION_TASK_STACK_SIZE         (MICROJVM_STACK_SIZE)
    #define VALIDATION_TASK_NAME               "BoardValidation"

#else
    /* MicroJvm task constants */
    #define JAVA_TASK_PRIORITY                 ( 8 ) /** Should be > tskIDLE_PRIORITY & < configTIMER_TASK_PRIORITY */
    #define JAVA_TASK_STACK_SIZE               (MICROJVM_STACK_SIZE)
    #define JAVA_TASK_NAME                     "MicroJvm"
#endif

/******************************************************
 *               Static Function Declarations
 ******************************************************/

void __attribute__((weak)) LLCOMM_stack_initialize(void)
{
    /* Does nothing by default, override when ECOM-COMM is used */
}

#ifdef VALIDATION_BUILD

static void xValidationTaskFunction(void * pvParameters)
{
    T_CORE_main();
    vTaskDelete(xTaskGetCurrentTaskHandle());
}

#else

static void xJavaTaskFunction(void * pvParameters)
{
    LLCOMM_stack_initialize();
    microej_main(0, NULL);
    vTaskDelete(xTaskGetCurrentTaskHandle());
}

#endif

static void reset_reason(esp_reset_reason_t reason)
{
    switch (reason)
    {
    case ESP_RST_UNKNOWN:
        printf("Reset reason can not be determined. \n");
        break;
    case ESP_RST_POWERON:
        printf("Reset due to power-on event. \n");
        break;
    case ESP_RST_EXT:
        printf("Reset by external pin. \n");
        break;
    case ESP_RST_SW:
        printf("Software reset via esp_restart \n");
        break;
    case ESP_RST_PANIC:
        printf("Software reset due to exception/panic. \n");
        break;
    case ESP_RST_INT_WDT:
        printf("Reset (software or hardware) due to interrupt watchdog. \n");
        break;
    case ESP_RST_TASK_WDT:
        printf("Reset due to task watchdog. \n");
        break;
    case ESP_RST_WDT:
        printf("Reset due to other watchdogs. \n");
        break;
    case ESP_RST_DEEPSLEEP:
        printf("Reset after exiting deep sleep mode. \n");
        break;
    case ESP_RST_BROWNOUT:
        printf("Brownout reset (software or hardware) \n");
        break;
    case ESP_RST_SDIO:
        printf("Reset over SDIO. \n");
        break;
    default:
        printf("NO_MEAN \n");
    }
}

static void print_reset_reason(void)
{
    esp_reset_reason_t rst_reson;
    rst_reson = esp_reset_reason();
    printf("Reset reason:");
    //print the reset reason for CPU 0
    reset_reason(rst_reson);
}

void app_main()
{
#ifndef VALIDATION_BUILD
    uart_switch();
#endif

    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU cores, WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%ldMB %s flash\n", flash_size / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    print_reset_reason();

    /* Initialize NVS */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    /* Print partition information */
    const esp_partition_t *partition = esp_ota_get_running_partition();
    if (partition != NULL) {
        switch (partition->subtype) {
        case ESP_PARTITION_SUBTYPE_APP_FACTORY:
            printf("Running application from factory partition\n");
            break;
        case ESP_PARTITION_SUBTYPE_APP_OTA_0:
            printf("Running application from ota_0 partition\n");
            break;
        case ESP_PARTITION_SUBTYPE_APP_OTA_1:
            printf("Running application from ota_1 partition\n");
            break;
        default:
            printf("Running application from unknown partition\n");
            break;
        }
    } else {
        printf("Cannot retrieve running partition\n");
    }

    /* Start the MicroJvm thread or the Validation thread */
    TaskHandle_t pvCreatedTask;

#ifndef VALIDATION_BUILD
    xTaskCreate(xJavaTaskFunction, JAVA_TASK_NAME, JAVA_TASK_STACK_SIZE, NULL, JAVA_TASK_PRIORITY, &pvCreatedTask);
#else
    xTaskCreate(xValidationTaskFunction, VALIDATION_TASK_NAME, VALIDATION_TASK_STACK_SIZE, NULL, VALIDATION_TASK_PRIORITY, &pvCreatedTask);
#endif

#ifndef VALIDATION_BUILD
#if CONFIG_APPTRACE_SV_ENABLE == 1
start_sysview_logging:
    SEGGER_SYSVIEW_setMicroJVMTask((U32)pvCreatedTask);
#endif // CONFIG_APPTRACE_SV_ENABLE
#endif
}

#ifndef VALIDATION_BUILD
#include "sni.h"

jfloat Java_com_microej_core_tests_MicroejCoreValidation_testFloat(jfloat a, jfloat b){
    return a * b;
}

jdouble Java_com_microej_core_tests_MicroejCoreValidation_testDouble(jdouble a, jdouble b){
    return a * b;
}

jint Java_com_microej_core_tests_MicroejCoreValidation_testNativeArguments01(jint i1, jint i2, jint i3, jint i4, jint i5, jint i6, jint i7, jint i8, jint i9, jint i10){
    if(i1==0x01020304 && 
       i2==0x05060708 && 
       i3==0x090A0B0C && 
       i4==0x0D0E0F10 && 
       i5==0x11121314 && 
       i6==0x15161718 && 
       i7==0x191A1B1C && 
       i8==0x1D1E1F20 && 
       i9==0x21222324 && 
       i10==0x25262728){
        return 0x292A2B2C;
    }
    else {
        return 0;
    }
}

jlong Java_com_microej_core_tests_MicroejCoreValidation_testNativeArguments02(jlong l1, jlong l2, jlong l3, jlong l4, jlong l5, jlong l6, jlong l7, jlong l8, jlong l9, jlong l10){
    if(l1==0x2D2E2F3031323334ll && 
       l2==0x35363738393A3B3Cll && 
       l3==0x3D3E3F4041424344ll && 
       l4==0x45464748494A4B4Cll && 
       l5==0x4D4E4F5051525354ll && 
       l6==0x55565758595A5B5Cll && 
       l7==0x5D5E5F6061626364ll && 
       l8==0x65666768696A6B6Cll && 
       l9==0x6D6E6F7071727374ll && 
       l10==0x75767778797A7B7Cll){
        return 0x7D7E7F8081828384ll;
    }
    else {
        return 0ll;
    }
}

jlong Java_com_microej_core_tests_MicroejCoreValidation_testNativeArguments03(jint i1, jlong l2, jint i3, jlong l4, jint i5, jlong l6, jint i7, jlong l8, jint i9, jlong l10){
    if(i1==0x85868788 && 
       l2==0x898A8B8C8D8E8F90ll && 
       i3==0x91929394 && 
       l4==0x95969798999A9B9Cll && 
       i5==0x9D9E9FA0 && 
       l6==0xA1A2A3A4A5A6A7A8ll && 
       i7==0xA9AAABAC && 
       l8==0xADAEAFB0B1B2B3B4ll && 
       i9==0xB5B6B7B8 && 
       l10==0xB9BABBBCBDBEBFC0ll){
        return 0xC1C2C3C4C5C6C7C8ll;
    }
    else {
        return 0ll;
    }
}

jfloat Java_com_microej_core_tests_MicroejCoreValidation_testNativeArguments04(jfloat f1, jfloat f2, jfloat f3, jfloat f4, jfloat f5, jfloat f6, jfloat f7, jfloat f8, jfloat f9, jfloat f10){
    if(f1==1.0f && 
       f2==1.1f && 
       f3==1.2f && 
       f4==1.3f && 
       f5==1.4f && 
       f6==1.5f && 
       f7==1.6f && 
       f8==1.7f && 
       f9==1.8f && 
       f10==1.9f){
        return 2.0f;
    }
    else {
        return 0.0f;
    }
}

jdouble Java_com_microej_core_tests_MicroejCoreValidation_testNativeArguments05(jdouble d1, jdouble d2, jdouble d3, jdouble d4, jdouble d5, jdouble d6, jdouble d7, jdouble d8, jdouble d9, jdouble d10){
    if(d1==2.0 && 
       d2==2.1 && 
       d3==2.2 && 
       d4==2.3 && 
       d5==2.4 && 
       d6==2.5 && 
       d7==2.6 && 
       d8==2.7 && 
       d9==2.8 && 
       d10==2.9){
        return 3.0;
    }
    else {
        return 0.0;
    }
}

jdouble Java_com_microej_core_tests_MicroejCoreValidation_testNativeArguments06(jfloat f1, jdouble d2, jfloat f3, jdouble d4, jfloat f5, jdouble d6, jfloat f7, jdouble d8, jfloat f9, jdouble d10){
    if(f1==3.0f && 
       d2==3.1 && 
       f3==3.2f && 
       d4==3.3 && 
       f5==3.4f && 
       d6==3.5 && 
       f7==3.6f && 
       d8==3.7 && 
       f9==3.8f && 
       d10==3.9){
        return 4.0;
    }
    else {
        return 0.0;
    }
}
#endif
