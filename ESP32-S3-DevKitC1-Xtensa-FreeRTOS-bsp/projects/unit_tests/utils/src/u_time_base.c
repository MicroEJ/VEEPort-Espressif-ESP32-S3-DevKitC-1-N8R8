/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "esp_timer.h"
#include "u_time_base.h"
#include "bsp_util.h"
#include "config.h"

/* Private constant declarations */


/* private variable definitions */


/* public function definitions */

BSP_DECLARE_WEAK_FCNT void UTIL_TIME_BASE_initialize(void)
{
}	

BSP_DECLARE_WEAK_FCNT int64_t UTIL_TIME_BASE_getTime(void)
{
	return (esp_timer_get_time()); /* returns the time in microseconds */
}

/** private function definitions */

