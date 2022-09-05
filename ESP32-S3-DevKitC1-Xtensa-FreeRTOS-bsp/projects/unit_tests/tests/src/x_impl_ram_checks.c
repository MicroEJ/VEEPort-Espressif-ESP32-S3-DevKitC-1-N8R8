/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "x_ram_checks.h"
#include "bsp_util.h"

/** @brief Definition for the RAM zones array */
X_RAM_CHECKS_zone_t RAM_zones[1];

/* public weak function default definitions */

/**
 * 
 * @brief  this function provide a definition array of  memory zones to test with 32-bit accesses.
 *
 * @return array of X_RAM_CHECKS_zone_t
 */

BSP_DECLARE_WEAK_FCNT X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get32bitZones(void)
{
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}


/**
 * @brief this function provide a definition array of  memory zones to test with 16-bit accesses.
 *
 * @return array of X_RAM_CHECKS_zone_t
 */

BSP_DECLARE_WEAK_FCNT X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get16bitZones(void)
{
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}


/**
 * @brief 
 *
 * @return array of X_RAM_CHECKS_zone_t
 */

BSP_DECLARE_WEAK_FCNT X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get8bitZones(void)
{
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}


/**
 * @brief 
 *
 * @return number of zones to test
 */

BSP_DECLARE_WEAK_FCNT uint8_t X_RAM_CHECKS_get32bitZoneNumber(void)
{
	return (sizeof(RAM_zones) / sizeof(RAM_zones[0]));
}


/**
 * @brief 
 *
 * @return 
 */

BSP_DECLARE_WEAK_FCNT uint8_t X_RAM_CHECKS_get16bitZoneNumber(void)
{
	return (sizeof(RAM_zones) / sizeof(RAM_zones[0]));
}


/**
 * @brief 
 *
 * @return 
 */

BSP_DECLARE_WEAK_FCNT uint8_t X_RAM_CHECKS_get8bitZoneNumber(void)
{
	return (sizeof(RAM_zones) / sizeof(RAM_zones[0]));
}


/**
 * @brief 
 *
 * @return 
 */

BSP_DECLARE_WEAK_FCNT X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get32bitSourceZone(void)
{
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}


/**
 * @brief 
 *
 * @return 
 */

BSP_DECLARE_WEAK_FCNT X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get16bitSourceZone(void)
{
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}


/**
 * @brief 
 *
 * @return 
 */

BSP_DECLARE_WEAK_FCNT X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get8bitSourceZone(void)
{
		return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}
