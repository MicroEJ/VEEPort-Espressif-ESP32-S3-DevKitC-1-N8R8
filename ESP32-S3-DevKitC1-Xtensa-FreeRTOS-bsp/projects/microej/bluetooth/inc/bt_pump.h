/*
 * C
 *
 * Copyright 2019-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#ifndef BT_PUMP_H
#define BT_PUMP_H

/**
 * @file
 * @brief Bluetooth pump interface.
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 2 May 2023
 */

#include <stdint.h>
#include "LLBLUETOOTH_defines.h"

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Starts the Bluetooth pump task.
 *
 * @return true on success, false on error.
 */
bool BT_PUMP_start(void);

/**
 * @brief Stops the Bluetooth pump task.
 *
 * @return true on success, false on error.
 */
void BT_PUMP_stop(void);

/**
 * @brief Gets the next Bluetooth event from the event queue. If there is no event in the queue, the
 * current Java thread is paused.
 *
 * @param[in] event          the buffer where to store the LLBLUETOOTH event.
 * @param[in] max_event_size the maximum event size that can be stored in the event buffer.
 *
 * @return event size in bytes, 0 if no event can be retrieved and the current Java thread paused.
 */
int32_t BT_PUMP_wait_event(LLBLUETOOTH_event_t *event, uint32_t max_event_size);

/**
 * @brief Sets the given scan filter. Scan result which do not fit this filter will not be returned in
 * BT_PUMP_wait_event().
 *
 * @param[in] filter_action    the filter action.
 * @param[in] filter_type      the filter type.
 * @param[in] filter_data      the filter data.
 * @param[in] filter_data_size the size of the filter data.
 */
void BT_PUMP_set_scan_filter(uint8_t filter_action, uint8_t filter_type, const uint8_t *filter_data,
		uint32_t filter_data_size);

#ifdef __cplusplus
	}
#endif

#endif /* BT_PUMP_H */
