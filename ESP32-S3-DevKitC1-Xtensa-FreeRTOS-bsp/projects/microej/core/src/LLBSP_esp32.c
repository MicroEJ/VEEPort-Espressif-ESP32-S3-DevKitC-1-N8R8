/*
 * C
 *
 * Copyright 2018-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/******************************************************************************
Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "LLBSP_impl.h"
#include "microej.h"
#include <stdio.h>
#include "esp_log.h"
#include "esp32s3/rom/ets_sys.h"
#include "esp_cpu.h"

extern void* SOAR_START;
extern void* SOAR_END;

/*******************************************************************************
 * Function Name: LLBSP_IMPL_isInReadOnlyMemory
 * Description  : checks if the given pointer is in a read only memory or not.
 *******************************************************************************/
uint8_t LLBSP_IMPL_isInReadOnlyMemory(void* ptr)
{
	if ((uint32_t)ptr >= (uint32_t)&SOAR_START && (uint32_t)ptr <= (uint32_t)&SOAR_END)
	{
		return MICROEJ_TRUE;
	}
	// RAM
	return MICROEJ_FALSE;
}

/*******************************************************************************
 * Function Name: LLBSP_IMPL_putchar
 * Description  : puts the character to stdout stream.
 *******************************************************************************/
void LLBSP_IMPL_putchar(int32_t c)
{
	ets_write_char_uart(c);
}

/*******************************************************************************
 * Function Name: LLBSP_IMPL_atomic_exchange
 * Description  : performs an atomic exchange
 *******************************************************************************/
int32_t LLBSP_IMPL_atomic_exchange(int32_t* ptr, int32_t value) {
    volatile uint32_t old = *((volatile uint32_t*)ptr);
    while (!esp_cpu_compare_and_set((volatile uint32_t*)ptr, old, (uint32_t)value)) {
        // we lost a race setting *ptr
        old = *((volatile uint32_t*)ptr);
    }
    return old;
}

/* End of File */
