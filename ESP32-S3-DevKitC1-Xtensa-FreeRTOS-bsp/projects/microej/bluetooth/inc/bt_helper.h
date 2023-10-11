/*
 * C
 *
 * Copyright 2019-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#ifndef BT_HELPER_H
#define BT_HELPER_H

/**
 * @file
 * @brief Bluetooth helper interface.
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 2 May 2023
 */

#include "esp_gap_ble_api.h"
#include "esp_bt_defs.h"
#include "esp_gatt_defs.h"
#include "LLBLUETOOTH_defines.h"
#include "bt_data.h"

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Converts the given Bluetooth address from esp-idf format to LLBLUETOOTH format.
 *
 * @param[in] addr      the bluetooth address in esp-idf format.
 * @param[in] addr_type the bluetooth address type.
 *
 * @return the bluetooth address in LLBLUETOOTH format.
 */
LLBLUETOOTH_address_t BT_HELPER_write_addr(const uint8_t *addr, esp_ble_addr_type_t addr_type);

/**
 * @brief Converts the given Bluetooth UUID from esp-idf format to LLBLUETOOTH format.
 *
 * @param[in] uuid the bluetooth UUID in esp-idf format.
 *
 * @return the bluetooth UUID in LLBLUETOOTH format.
 */
LLBLUETOOTH_uuid_t BT_HELPER_write_uuid(const esp_bt_uuid_t *uuid);

/**
 * @brief Converts the given Bluetooth address from LLBLUETOOTH format to esp-idf format.
 *
 * @param[in] lladdr    the bluetooth address in LLBLUETOOTH format.
 * @param[out] addr     output buffer where to save the bluetooth address in esp-idf format.
 * @param[in] addr_type the bluetooth address type.
 */
void BT_HELPER_read_addr(const LLBLUETOOTH_address_t *lladdr, uint8_t *addr, esp_ble_addr_type_t *addr_type);

/**
 * @brief Converts the given Bluetooth UUID from LLBLUETOOTH format to esp-idf format.
 *
 * @param[in] lluuid the bluetooth UUID in LLBLUETOOTH format.
 *
 * @return the bluetooth UUID in esp-idf format.
 */
esp_bt_uuid_t BT_HELPER_read_uuid(const LLBLUETOOTH_uuid_t *lluuid);

/**
 * @brief Converts the given Bluetooth permissions from LLBLUETOOTH format to esp-idf format.
 *
 * @param[in] llperm the bluetooth permissions in LLBLUETOOTH format.
 *
 * @return the bluetooth permissions in esp-idf format.
 */
esp_gatt_perm_t BT_HELPER_read_permissions(uint8_t llperm);

/**
 * @brief Converts the given Bluetooth service from LLBLUETOOTH format to esp-idf format.
 *
 * @param[in] llservice the bluetooth service in LLBLUETOOTH format.
 *
 * @return the bluetooth service in esp-idf format.
 */
BT_DATA_service_t BT_HELPER_read_service(const LLBLUETOOTH_gatts_service_t *llservice);

/**
 * @brief Converts the given Bluetooth characteristic from LLBLUETOOTH format to esp-idf format.
 *
 * @param[in] llattribute the bluetooth characteristic in LLBLUETOOTH format.
 *
 * @return the bluetooth characteristic in esp-idf format.
 */
BT_DATA_characteristic_t BT_HELPER_read_characteristic(const LLBLUETOOTH_gatts_attribute_t *llattribute);

/**
 * @brief Converts the given Bluetooth descriptor from LLBLUETOOTH format to esp-idf format.
 *
 * @param[in] llattribute the bluetooth descriptor in LLBLUETOOTH format.
 *
 * @return the bluetooth descriptor in esp-idf format.
 */
BT_DATA_descriptor_t BT_HELPER_read_descriptor(const LLBLUETOOTH_gatts_attribute_t *llattribute);

#ifdef __cplusplus
	}
#endif

#endif /* BT_HELPER_H */
