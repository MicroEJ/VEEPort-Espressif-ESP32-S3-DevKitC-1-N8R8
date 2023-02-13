/*
 * C
 *
 * Copyright 2014-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "x_ram_checks.h"
#include "bsp_util.h"

#include "esp_chip_info.h"
#include "esp_task_wdt.h"

/* Define the RAM zone structure on which the tests will run */
static X_RAM_CHECKS_zone_t RAM_zones[1];

/* Lazy initialization flag */
static bool initialize;

/* private function */
static void initialize_ram_zones(void) {
	if(initialize == false)	{
		initialize = true;

		esp_task_wdt_deinit();

		uint32_t heap_max_free_size = heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM);

		uint8_t* start_address_p = heap_caps_malloc(heap_max_free_size, MALLOC_CAP_SPIRAM);
		if (start_address_p != NULL) {
			/* We populate the RAM zones array definition with start and length */
			RAM_zones[0].start_address = (uint32_t)(start_address_p);
			RAM_zones[0].end_address = (uint32_t)start_address_p + heap_max_free_size - 1;
		} else {
			printf("Failed to allocate SPIRAM memory for tests!\n");
		}
	}
}

/* public function default definitions */

/**
 *
 * @brief  this function provide a definition array of  memory zones to test with 32-bit accesses.
 *
 * @return array of X_RAM_CHECKS_zone_t
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get32bitZones(void)
{
	initialize_ram_zones();
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}

/**
 * @brief this function provide a definition array of  memory zones to test with 16-bit accesses.
 *
 * @return array of X_RAM_CHECKS_zone_t
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get16bitZones(void)
{
	initialize_ram_zones();
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}

/**
 * @brief
 *
 * @return array of X_RAM_CHECKS_zone_t
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get8bitZones(void)
{
	initialize_ram_zones();
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}

/**
 * @brief
 *
 * @return number of zones to test
 */
uint8_t X_RAM_CHECKS_get32bitZoneNumber(void)
{
	initialize_ram_zones();
	return (sizeof(RAM_zones) / sizeof(RAM_zones[0]));
}

/**
 * @brief
 *
 * @return
 */
uint8_t X_RAM_CHECKS_get16bitZoneNumber(void)
{
	initialize_ram_zones();
	return (sizeof(RAM_zones) / sizeof(RAM_zones[0]));
}

/**
 * @brief
 *
 * @return
 */
uint8_t X_RAM_CHECKS_get8bitZoneNumber(void)
{
	initialize_ram_zones();
	return (sizeof(RAM_zones) / sizeof(RAM_zones[0]));
}

/**
 * @brief
 *
 * @return
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get32bitSourceZone(void)
{
	initialize_ram_zones();
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}

/**
 * @brief
 *
 * @return
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get16bitSourceZone(void)
{
	initialize_ram_zones();
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}

/**
 * @brief
 *
 * @return
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get8bitSourceZone(void)
{
	initialize_ram_zones();
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}
