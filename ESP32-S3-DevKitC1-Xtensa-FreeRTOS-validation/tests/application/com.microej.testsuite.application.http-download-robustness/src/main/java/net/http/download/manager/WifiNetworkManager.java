/*
 * Java
 *
 * Copyright 2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
package net.http.download.manager;

import java.io.IOException;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.logging.Logger;

import ej.ecom.network.IPConfiguration;
import ej.ecom.network.NetworkInterfaceManager;
import ej.ecom.wifi.AccessPoint;
import ej.ecom.wifi.SecurityMode;
import ej.ecom.wifi.WifiManager;
import net.http.download.Config;

/**
 * This is the Wi-Fi network manager.
 */
public class WifiNetworkManager implements IWifiNetworkManager {

	/**
	 * Logger
	 */
	private static final Logger LOGGER = Logger.getLogger(WifiNetworkManager.class.getName());

	/**
	 * Network interface name property
	 */
	private static final String INTERFACE_NAME_PROPERTY = "wifi.interface.name"; //$NON-NLS-1$

	/**
	 * Logging constant.
	 */
	private static final String MANUALLY = "manually";

	/**
	 * Logging constant.
	 */
	private static final String BY_DHCP = "by DHCP";

	/**
	 * Network interface name
	 */
	private final String interfaceName = System.getProperty(INTERFACE_NAME_PROPERTY);

	/**
	 * Wi-Fi network interface.
	 */
	private NetworkInterface wifi = null;

	/**
	 * Wi-Fi manager.
	 */
	private WifiManager wifiManager = null;

	/**
	 * Initializes the network
	 *
	 * @throws IOException
	 *             if any exception occurs when initializing the network
	 */
	private void initNetwork() throws IOException {
		LOGGER.info("Initializes the Wi-Fi network"); //$NON-NLS-1$

		/**
		 * Check if Wi-Fi interface is found
		 */
		this.wifi = NetUtils.findInterface(this.interfaceName);
		if (this.wifi == null) {
			throw new IOException("No Wi-Fi interface found"); //$NON-NLS-1$
		}

		/**
		 * Initialize the Wi-Fi manager
		 */
		this.wifiManager = WifiManager.getInstance();

		/**
		 * Start Wi-Fi interface
		 */
		if (!NetworkInterfaceManager.isStarted(this.wifi)) {
			NetworkInterfaceManager.start(this.wifi);
		}

		/**
		 * Enable Wi-Fi interface
		 */
		if (!NetworkInterfaceManager.isEnabled(this.wifi)) {
			NetworkInterfaceManager.enable(this.wifi);
		}
	}

	/**
	 * Joins the network
	 *
	 * @throws IOException
	 *             if any exception occurs when joining the network
	 */
	private void joinNetwork() throws IOException {
		IPConfiguration userIPConfig;
		IPConfiguration wifiIPConfig;
		InetAddress[] dnsList;
		AccessPoint joinedAP;

		/**
		 * Perform IP configuration
		 */
		userIPConfig = new IPConfiguration();
		userIPConfig.useDHCP(true);
		userIPConfig.useStaticDNS(false);
		NetworkInterfaceManager.configure(this.wifi, userIPConfig);

		/**
		 * Joins the configured Access Point
		 */
		LOGGER.info("Joining Wi-Fi Access Point"); //$NON-NLS-1$

		/**
		 * Joins the configured Access Point
		 */
		String ssid = System.getProperty(Config.WIFI_SSID);
		String password = System.getProperty(Config.WIFI_PASSPHRASE);
		int security = Integer.parseInt(System.getProperty(Config.WIFI_SECURITY));
		this.wifiManager.join(ssid, password, SecurityMode.values()[security]);

		/**
		 * Wait until an IP address is assigned
		 */
		NetUtils.waitUntilIpIsAssigned(this.interfaceName);

		/**
		 * Get IP configuration
		 */
		wifiIPConfig = NetworkInterfaceManager.getIPConfiguration(this.wifi);
		dnsList = wifiIPConfig.getDNS();

		/**
		 * Print IP configuration
		 */
		LOGGER.info("My IP address: " + wifiIPConfig.getIP() + " (assigned " //$NON-NLS-1$//$NON-NLS-2$
				+ (wifiIPConfig.isStatic() ? MANUALLY : BY_DHCP) + ")"); //$NON-NLS-1$
		LOGGER.info("My netmask: " + wifiIPConfig.getNetmask() + " (assigned " //$NON-NLS-1$//$NON-NLS-2$
				+ (wifiIPConfig.isStatic() ? MANUALLY : BY_DHCP) + ")"); //$NON-NLS-1$
		LOGGER.info("My gateway: " + wifiIPConfig.getGateway() + " (assigned " //$NON-NLS-1$ //$NON-NLS-2$
				+ (wifiIPConfig.isStatic() ? MANUALLY : BY_DHCP) + ")"); //$NON-NLS-1$
		LOGGER.info("My DNS list (assigned " + (wifiIPConfig.isStatic() ? MANUALLY : BY_DHCP) + "):"); //$NON-NLS-1$ //$NON-NLS-2$
		for (int i = 0; i < dnsList.length; i++) {
			LOGGER.info("    * " + dnsList[i]); //$NON-NLS-1$
		}

		/**
		 * Check if the AP is actually joined
		 */
		joinedAP = this.wifiManager.getJoined();
		if (joinedAP != null) {
			LOGGER.info("Access Point joined");
		} else {
			throw new IOException("Access Point not joined");
		}
	}

	@Override
	public void init() throws IOException {
		initNetwork();
		joinNetwork();
		setDefault();
		NetUtils.updateLocalTime();
	}

	@Override
	public void deinit() throws IOException {
		if (this.wifiManager != null) {
			this.wifiManager.leave();
			this.wifiManager = null;
		}

		if (this.wifi != null) {
			this.wifi = null;
		}
	}

	@Override
	public void setDefault() throws IOException {
		NetworkInterfaceManager.setDefault(this.wifi);
	}
}
