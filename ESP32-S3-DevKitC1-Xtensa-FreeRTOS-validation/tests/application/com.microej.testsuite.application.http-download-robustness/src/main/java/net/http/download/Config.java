/*
 * Java
 *
 * Copyright 2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
package net.http.download;

/**
 * This class provides example configuration.
 */
public class Config {

	/**
	 * Access Point Name.
	 */
	public static final String MOBILE_APN = "mobile.apn";

	/**
	 * Username to connect to the APN.
	 */
	public static final String MOBILE_USER = "mobile.user";

	/**
	 * Passphrase to connect to the APN.
	 */
	public static final String MOBILE_PASSPHRASE = "mobile.passphrase";

	/**
	 * PIN code to unlock the SIM card.
	 */
	public static final String MOBILE_PIN_CODE = "mobile.pin.code";

	/**
	 * Wi-Fi SSID.
	 */
	public static final String WIFI_SSID = "wifi.ssid";

	/**
	 * Passphrase credential for Wi-Fi connection.
	 */
	public static final String WIFI_PASSPHRASE = "wifi.passphrase";

	/**
	 * Security mode for Wi-Fi connection (MIXED (WPA1 / WPA2)=6; OPEN=7; WPA1=9;
	 * WPA2=10; WPA3=11; MIXED (WPA2 / WPA3)=12).
	 */
	public static final String WIFI_SECURITY = "wifi.security";

	/**
	 * Avoid the config class to be instantiated.
	 */
	private Config() {

	}
}