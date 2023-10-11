/*
 * C
 *
 * Copyright 2019-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#ifndef BT_MANAGER_H
#define BT_MANAGER_H

/**
 * @file
 * @brief Bluetooth manager interface.
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 2 May 2023
 */

#include <stdbool.h>
#include <stdint.h>
#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"
#include "bt_data.h"

#ifdef __cplusplus
	extern "C" {
#endif

/** @brief Enable or disable LLBLUETOOTH debugging. */
#define LLBLUETOOTH_DEBUG

/** @brief Debugging macro. */
#ifdef LLBLUETOOTH_DEBUG
	#include <stdio.h>
	#define LLBLUETOOTH_DEBUG_TRACE			(void)printf
#else
	#define LLBLUETOOTH_DEBUG_TRACE(...)	((void) 0)
#endif

/**
 * @brief Enables the Bluetooth stack.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_enable(void);

/**
 * @brief Disables the Bluetooth stack.
 *
 * @return true on success, false on error.
 */
void BT_MANAGER_disable(void);

/**
 * @brief Gets the GATTS interface handle.
 *
 * @return the GATTS interface handle.
 */
esp_gatt_if_t BT_MANAGER_get_gatts_if(void);

/**
 * @brief Gets the GATTC interface handle.
 *
 * @return the GATTC interface handle.
 */
esp_gatt_if_t BT_MANAGER_get_gattc_if(void);

/**
 * @brief Sets the GATTS interface handle.
 *
 * @param[in] gatts_if the GATTS interface handle.
 */
void BT_MANAGER_set_gatts_if(esp_gatt_if_t gatts_if);

/**
 * @brief Sets the GATTC interface handle.
 *
 * @param[in] gattc_if the GATTC interface handle.
 */
void BT_MANAGER_set_gattc_if(esp_gatt_if_t gattc_if);

/**
 * @brief Sets the attribute handle of a prepare write request.
 *
 * @param[in] attr_handle the attribute handle.
 */
void BT_MANAGER_set_prepare_write_attr_handle(uint16_t attr_handle);

/**
 * @brief Sets the transfer id of a prepare write request.
 *
 * @param[in] trans_id the transfer id.
 */
void BT_MANAGER_set_prepare_write_trans_id(uint32_t trans_id);

/**
 * @brief Gets the attribute handle of a prepare write request.
 *
 * @param[out] attr_handle the attribute handle.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_get_prepare_write_attr_handle(uint16_t *attr_handle);

/**
 * @brief Gets the address of the device with which a connection has been initiated.
 *
 * @param[out] addr      the address of the device.
 * @param[out] addr_type the address type.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_get_connecting_device(uint8_t *addr, esp_ble_addr_type_t *addr_type);

/**
 * @brief Gets the address of the device with which a connection has been established.
 *
 * @param[out] addr      the address of the device.
 * @param[out] addr_type the address type.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_get_connected_device(uint8_t *addr, esp_ble_addr_type_t *addr_type);

/**
 * @brief Gets the connection handle of the connection which has been established.
 *
 * @param[out] conn_id the connection handle.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_get_connected_conn_id(uint16_t *conn_id);

/**
 * @brief Gets the attribute handle of the characteristic to which a notification or indication has been sent.
 *
 * @param[out] attr_handle the attribute handle.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_get_notification_attr_handle(uint16_t *attr_handle);

/**
 * @brief Called when a connection has been established.
 *
 * @param[in] conn_id   the connection id.
 * @param[in] addr      the address of the connected device.
 & @param[in] addr_type the address type.
 */
void BT_MANAGER_on_connected(uint16_t conn_id, const uint8_t *addr, esp_ble_addr_type_t *addr_type);

/**
 * @brief Called when a connection procedure has been failed.
 */
void BT_MANAGER_on_connect_failed(void);

/**
 * @brief Called when a connection has been terminated.
 */
void BT_MANAGER_on_disconnected(void);

/**
 * @brief Called when an attribute or service has been created.
 *
 * @param[in] status the status of attribute creation.
 * @param[in] handle the attribute handle.
 */
void BT_MANAGER_on_attribute_created(esp_gatt_status_t status, uint16_t handle);

/**
 * @brief Called when an attribute read/write request has been received.
 *
 * @param[in] handle   the attribute handle.
 * @param[in] trans_id the transfer id.
 */
void BT_MANAGER_on_attribute_request(uint16_t handle, uint32_t trans_id);

/**
 * @brief Called when a characteristic has been discovered.
 *
 * @param[in] handle the characteristic handle.
 */
void BT_MANAGER_on_characteristic_discovered(uint16_t handle);

/**
 * @brief Adds the given GATT service.
 *
 * @param[in] s the GATT service.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_add_service(BT_DATA_service_t *s);

/**
 * @brief Adds the given GATT characteristic.
 *
 * @param[in[ service_handle the service handle to which to add the characteristic.
 * @param[in] c              the GATT characteristic.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_add_characteristic(uint16_t service_handle, BT_DATA_characteristic_t *c);

/**
 * @brief Adds the given GATT descriptor.
 *
 * @param[in[ service_handle the service handle to which to add the descriptor.
 * @param[in] d              the GATT descriptor.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_add_descriptor(uint16_t service_handle, BT_DATA_descriptor_t *d);

/**
 * @brief Starts/registers the GATT service with the given handle.
 *
 * @param[in[ service_handle the service handle.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_start_service(uint16_t service_handle);

/**
 * @brief Starts scanning.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_start_scan(void);

/**
 * @brief Stops scanning.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_stop_scan(void);

/**
 * @brief Starts advertising using the given advertising data.
 *
 * @param[in] adv_data      advertising data.
 * @param[in] adv_data_size the size of the advertising data.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_start_adv(const uint8_t *adv_data, uint8_t adv_data_size);

/**
 * @brief Stops advertising.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_stop_adv(void);

/**
 * @brief Connects to the device with the given address.
 *
 * @param[in] addr      the address of the device to connect.
 * @param[in] addr_type the address type.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_connect(const uint8_t *addr, esp_ble_addr_type_t addr_type);

/**
 * @brief Disconnects from the device with the given connection handle.
 *
 * @param[in] conn_id the connection id.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_disconnect(uint16_t conn_id);

/**
 * @brief Sends a pair request to the device.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_send_pair_request(void);

/**
 * @brief Sends a pair response to the device.
 *
 * @param[in] accept true if the pairing request should be accepted, false otherwise.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_send_pair_response(bool accept);

/**
 * @brief Sends a passkey response to the device.
 *
 * @param[in] accept  true if the pairing request should be accepted, false otherwise.
 * @param[in] passkey the passkey value (6-digits PIN code).
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_send_passkey_response(bool accept, uint32_t passkey);

/**
 * @brief Discovers the services provided by the device with the given connection handle. If the given UUID
 * is NULL, all the services of the device are discovered, otherwise only the services with the given
 * UUID are discovered.
 *
 * @param[in] conn_id the connection id.
 * @param[in] uuid    the UUID of the service to be discovered (NULL if all services need to be discovered).
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_discover_services(uint16_t conn_id, const esp_bt_uuid_t *uuid);

/**
 * @brief Sends an read request to the device with the given connection handle on the attribute with the
 * given attribute handle.
 *
 * @param[in] conn_id     the connection id.
 * @param[in] attr_handle the attribute handle.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_send_read_request(uint16_t conn_id, uint16_t attr_handle);

/**
 * @brief Sends a write request with the given value to the device with the given connection handle on the
 * attribute with the given attribute handle.
 *
 * @param[in] conn_id     the connection id.
 * @param[in] attr_handle the attribute handle.
 * @param[in] value_size  the size of the value.
 * @param[in] value       the value to write.
 * @param[in] no_response true to use the write without response procedure, false to use the write procedure.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_send_write_request(uint16_t conn_id, uint16_t attr_handle, uint16_t value_size,
	const uint8_t *value, bool no_response);

/**
 * @brief Sends a read response with the given status and value to the device with the given connection
 * handle on the attribute with the given attribute handle.
 *
 * @param[in] conn_id     the connection id.
 * @param[in] attr_handle the attribute handle.
 * @param[in] status      the status to send back.
 * @param[in] value_size  the size of the value.
 * @param[in] value       the value to send back.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_send_read_response(uint16_t conn_id, uint16_t attr_handle, esp_gatt_status_t status,
	uint16_t value_size, const uint8_t *value);

/**
 * @brief Sends a write response with the given status to the device with the given connection handle on the
 * attribute with the given attribute handle.
 *
 * @param[in] conn_id     the connection id.
 * @param[in] attr_handle the attribute handle.
 * @param[in] status      the status to send back.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_send_write_response(uint16_t conn_id, uint16_t attr_handle, esp_gatt_status_t status);

/**
 * @brief Sends a prepare write response with the given data to the device with the given connection handle on the
 * attribute with the given attribute handle.
 *
 * @param[in] conn_id     the connection id.
 * @param[in] attr_handle the attribute handle.
 * @param[in] status      the status to send back.
 * @param[in] value_size  the size of the value of the prepare write request.
 * @param[in] value       the value of the prepare write request.
 * @param[in] offset      the offset of the prepare write request.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_send_prepare_write_response(uint16_t conn_id, uint16_t attr_handle, esp_gatt_status_t status,
		uint32_t value_size, const uint8_t *value, uint32_t offset);

/**
 * @brief Sends an execute write response with the given status to the device with the given connection handle on the
 * attribute with the given attribute handle.
 *
 * @param[in] conn_id     the connection id.
 * @param[in] attr_handle the attribute handle.
 * @param[in] status      the status to send back.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_send_execute_write_response(uint16_t conn_id, uint16_t attr_handle, esp_gatt_status_t status);

/**
 * @brief Sends a notification or indication with the given status and value to the device with the given
 * connection handle on the characteristic with the given attribute handle.
 *
 * @param[in] conn_id     the connection id.
 * @param[in] attr_handle the attribute handle.
 * @param[in] value_size  the size of the value.
 * @param[in] value       the value to send.
 * @param[in] confirm     true to request a confirmation (GATT indication), false otherwise (GATT notification).
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_send_notification(uint16_t conn_id, uint16_t attr_handle, uint16_t value_size,
	const uint8_t *value, bool confirm);

/**
 * @brief Gets the number of attributes within the given handle range.
 *
 * @param[in] conn_id         the connection id.
 * @param[in] start_handle    the attribute handle start range.
 * @param[in] end_handle      the attribute handle end range.
 * @param[out] num_attributes the number of attributes found.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_get_num_attributes(uint16_t conn_id, uint16_t start_handle, uint16_t end_handle,
		uint8_t *num_attributes);

/**
 * @brief Gets the next characteristic within the given handle range.
 *
 * @param[in] conn_id         the connection id.
 * @param[in] start_handle    the attribute handle start range.
 * @param[in] end_handle      the attribute handle end range.
 * @param[in] offset          the offset of the characteristic position to get.
 * @param[out] characteristic the characteristic at the given offset.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_get_characteristic(uint16_t conn_id, uint16_t start_handle, uint16_t end_handle,
		uint16_t offset, esp_gattc_char_elem_t *characteristic);

/**
 * @brief Gets the next descriptor within the given handle range.
 *
 * @param[in] conn_id     the connection id.
 * @param[in] char_handle the attribute handle start range.
 * @param[in] offset      the offset of the descriptor position to get.
 * @param[out] descriptor the descriptor at the given offset.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_get_descriptor(uint16_t conn_id, uint16_t char_handle, uint16_t offset,
		esp_gattc_descr_elem_t *descriptor);

/**
 * @brief Gets whether the attribute with the given handle provided by the device with the given connection
 * handle is a characteristic or a descriptor.
 *
 * @param[in] conn_id            the connection id.
 * @param[in] attr_handle        the attribute handle to check.
 * @param[out] is_characteristic true if the attribute with the given handle is a characteristic, false if it is a descriptor.
 *
 * @return true on success, false on error.
 */
bool BT_MANAGER_get_attribute_type(uint16_t conn_id, uint16_t attr_handle, bool *is_characteristic);

#ifdef __cplusplus
	}
#endif

#endif /* BT_MANAGER_H */
