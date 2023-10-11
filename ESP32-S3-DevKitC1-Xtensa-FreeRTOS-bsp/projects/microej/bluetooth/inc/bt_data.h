/*
 * C
 *
 * Copyright 2019-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#ifndef BT_DATA_H
#define BT_DATA_H

/**
 * @file
 * @brief Bluetooth data related types.
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 2 May 2023
 */

#include "esp_bt_defs.h"
#include "esp_gatt_defs.h"

#ifdef __cplusplus
	extern "C" {
#endif

/** @brief Defines a GATT service. */
typedef struct {
	esp_bt_uuid_t uuid;
	uint8_t num_char;
	uint8_t num_desc;
	uint16_t handle;
} BT_DATA_service_t;

/** @brief Defines a GATT characteristic. */
typedef struct {
	esp_bt_uuid_t uuid;
	esp_gatt_perm_t permissions;
	esp_gatt_char_prop_t properties;
	uint16_t value_handle;
} BT_DATA_characteristic_t;

/** @brief Defines a GATT descriptor. */
typedef struct {
	esp_bt_uuid_t uuid;
	esp_gatt_perm_t permissions;
	uint16_t handle;
} BT_DATA_descriptor_t;

#ifdef __cplusplus
	}
#endif

#endif /* BT_DATA_H */
