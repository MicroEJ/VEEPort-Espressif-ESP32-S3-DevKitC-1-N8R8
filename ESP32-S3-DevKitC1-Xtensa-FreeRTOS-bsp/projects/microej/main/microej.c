/*
 * C
 *
 * Copyright 2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_task_wdt.h"
#include "nvs_flash.h"
#include "microej_main.h"
#include "uart_switch.h"
#include "esp_ota_ops.h"
#include "esp_system.h"
#include "sdkconfig.h"

#if CONFIG_APPTRACE_SV_ENABLE
#include "SEGGER_SYSVIEW.h"
#endif // CONFIG_APPTRACE_SV_ENABLE


/******************************************************
 *                    Constants
 ******************************************************/

/* MicroJvm task constants */
#define MICROJVM_STACK_SIZE                (21 * 1024)
#define JAVA_TASK_PRIORITY                 ( 8 ) /** Should be > tskIDLE_PRIORITY & < configTIMER_TASK_PRIORITY */
#define JAVA_TASK_STACK_SIZE               (MICROJVM_STACK_SIZE / 4)
#define JAVA_TASK_NAME	                   "MicroJvm"

/******************************************************
 *               Static Function Declarations
 ******************************************************/

void __attribute__((weak)) LLCOMM_stack_initialize(void)
{
	/* Does nothing by default, override when ECOM-COMM is used */
}

static void xJavaTaskFunction(void * pvParameters)
{
	LLCOMM_stack_initialize();
	microej_main(0, NULL);
	vTaskDelete(xTaskGetCurrentTaskHandle());
}

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
	uart_switch();

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU cores, WiFi%s%s, ",
			CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
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

    /* Start the MicroJvm thread */
    TaskHandle_t pvCreatedTask;
    xTaskCreate(xJavaTaskFunction, JAVA_TASK_NAME, JAVA_TASK_STACK_SIZE, NULL, JAVA_TASK_PRIORITY, &pvCreatedTask);

#if CONFIG_APPTRACE_SV_ENABLE == 1
start_sysview_logging:
    SEGGER_SYSVIEW_setMicroJVMTask((U32)pvCreatedTask);
#endif // CONFIG_APPTRACE_SV_ENABLE
}

#include "soc/compare_set.h"
unsigned int __atomic_exchange_4(unsigned int *ptr, unsigned int val){
	uint32_t set = val;
	compare_and_set_native(ptr, *ptr, &set);
	return set;
}

