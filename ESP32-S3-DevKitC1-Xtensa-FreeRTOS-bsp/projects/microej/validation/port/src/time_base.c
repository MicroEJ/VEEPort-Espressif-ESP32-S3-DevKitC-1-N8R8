/*
 * C
 *
 * Copyright 2014-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "../../framework/c/utils/inc/u_time_base.h"
#include "../../framework/c/utils/inc/bsp_util.h"
#include "../../framework/c/embunit/embunit/config.h"

#include "esp_timer.h"

/* Private constant declarations */



/* private variable definitions */


/* public function definitions */

void UTIL_TIME_BASE_initialize(void)
{
}

int64_t UTIL_TIME_BASE_getTime(void)
{
	return (esp_timer_get_time()); /* returns the time in microseconds */
}

/** private function definitions */
