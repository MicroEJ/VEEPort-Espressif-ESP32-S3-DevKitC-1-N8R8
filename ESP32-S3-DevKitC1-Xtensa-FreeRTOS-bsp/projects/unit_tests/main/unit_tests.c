/*
 * C
 *
 * Copyright 2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_heap_caps.h"
#include "Outputter.h"
#include "t_core_main.h"
#include "x_ram_checks.h"

/******************************************************
 *                    Constants
 ******************************************************/

/* UnitTests task constants */
#define UNIT_TESTS_STACK_SIZE              (21 * 1024)
#define UNIT_TESTS_TASK_PRIORITY           ( CONFIG_TIMER_TASK_PRIORITY - 1 ) /** Should be > tskIDLE_PRIORITY & < configTIMER_TASK_PRIORITY */
#define UNIT_TESTS_TASK_STACK_SIZE         (UNIT_TESTS_STACK_SIZE / 4)
#define UNIT_TESTS_TASK_NAME	           "unit_tests"

/* Array with RAM zones definition defined in 'x_imp_ram_checks.c' */
extern X_RAM_CHECKS_zone_t RAM_zones[1];

/******************************************************
 *               Static Function Declarations
 ******************************************************/

static void xUnitTestsTaskFunction(void * pvParameters)
{
	printf("\n================================================================================\n");
	printf("SPIRAM capabilities: \n");
	heap_caps_print_heap_info(MALLOC_CAP_SPIRAM);
	uint32_t heap_total_size = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
	printf("SPIRAM heap total size: %u \n", heap_total_size);
	uint32_t heap_max_free_size = heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM);
	printf("SPIRAM heap maximum block free size: %u \n", heap_max_free_size);	
	
	uint8_t* start_address_p = heap_caps_malloc(heap_max_free_size, MALLOC_CAP_SPIRAM);
	if (start_address_p != NULL)
	{
		printf("Allocating memory at %X with size of %u.\n\n", (uint32_t)start_address_p, heap_max_free_size);

		/* We populate the RAM zones array definition with start and length */
		RAM_zones[0].start_address = (uint32_t)(start_address_p);
		RAM_zones[0].end_address = ((uint32_t)start_address_p + heap_max_free_size - 1);
		RAM_zones[0].write_speed = 0;
		RAM_zones[0].read_speed = 0;
		RAM_zones[0].transfert_speed = 0;

		printf("Start unit_tests benchmark\n");	
		T_CORE_main();
		printf("End unit_tests benchmark\n");

		do {
			vTaskDelay(1);
		} while (1);
	}
	else
	{
		printf("Failed to allocate SPIRAM memory for tests!\n");
	}
	
	vTaskDelete(xTaskGetCurrentTaskHandle());
}

void app_main()
{
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

    /* Initialize NVS */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    /* Start the unit_tests thread */
    xTaskCreate(xUnitTestsTaskFunction, UNIT_TESTS_TASK_NAME, UNIT_TESTS_TASK_STACK_SIZE, NULL, UNIT_TESTS_TASK_PRIORITY, NULL);
}
