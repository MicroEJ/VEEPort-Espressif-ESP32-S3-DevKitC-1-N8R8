/*
 * Java
 *
 * Copyright 2019 MicroEJ Corp. All rights reserved.
 * MicroEJ Corp. PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
package com.microej.esp32.ota_test;

import org.junit.Assert;
import org.junit.Test;

/**
 * Prints the message "Hello World !" an displays MicroEJ splash
 */

public class OtaTest {
	public static void main(String[] args) {
		new OtaTest().runTest();
	}

	@Test
	public void runTest() {
		// Detect if OTA is enabled.
		// This is a property that must be set by the harness.
		boolean ota_enabled = Boolean.parseBoolean(System.getProperty("ota_enabled", "true"));

		// Get pointer to the next update partition for OTA
		int running_partition = com.espressif.esp_idf.esp_ota_ops.esp_ota_get_running_partition();
		int update_partition = com.espressif.esp_idf.esp_ota_ops.esp_ota_get_next_update_partition(running_partition);

		if (ota_enabled) {
			// If OTA is enabled, the next update partition should not be NULL (0)
			Assert.assertNotEquals(update_partition, 0);
		} else {
			// If OTA is disbled, the next update partition should be NULL (0)
			Assert.assertEquals(update_partition, 0);
		}

		// Try to start an OTA update
		int ret = com.espressif.esp_idf.esp_ota_ops.esp_ota_begin(update_partition, 1024);

		if (ota_enabled) {
			// If OTA update is enabled, esp_ota_begin should return ESP_OK
			Assert.assertEquals(ret, com.espressif.esp_idf.esp_err.ESP_OK);
		} else {
			// If OTA update is disabled, esp_ota_begin should return ESP_ERR_INVALID_ARG
			Assert.assertEquals(ret, com.espressif.esp_idf.esp_err.ESP_ERR_INVALID_ARG);
		}
	}
}
