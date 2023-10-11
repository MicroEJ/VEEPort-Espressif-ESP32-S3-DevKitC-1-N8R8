/*
 * C
 *
 * Copyright 2019-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief Bluetooth write helper implementation.
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 2 May 2023
 */

#include <stdio.h>
#include <string.h>
#include "bt_helper.h"

#ifdef __cplusplus
	extern "C" {
#endif

LLBLUETOOTH_address_t BT_HELPER_write_addr(const uint8_t *addr, esp_ble_addr_type_t addr_type)
{
	LLBLUETOOTH_address_t lladdr;
	lladdr.type = (addr_type == BLE_ADDR_TYPE_PUBLIC) ? ADDRESS_TYPE_PUBLIC : ADDRESS_TYPE_PRIVATE;
	(void)memcpy(lladdr.addr, addr, ESP_BD_ADDR_LEN);
	return lladdr;
}

LLBLUETOOTH_uuid_t BT_HELPER_write_uuid(const esp_bt_uuid_t *uuid)
{
	LLBLUETOOTH_uuid_t lluuid;

	if (uuid->len == ESP_UUID_LEN_16) {
		const uint8_t standard_uuid[] = {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x10, 0x00,
				0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB};

		(void)memcpy(lluuid.uuid, standard_uuid, 16);
		lluuid.uuid[2] = ((uuid->uuid.uuid16 >> 8) & 0xFF);
		lluuid.uuid[3] = (uuid->uuid.uuid16 & 0xFF);
	} else {
		for (int i = 0; i < 16; i++) {
			lluuid.uuid[i] = uuid->uuid.uuid128[16-1-i];
		}
	}

	return lluuid;
}

#ifdef __cplusplus
	}
#endif
