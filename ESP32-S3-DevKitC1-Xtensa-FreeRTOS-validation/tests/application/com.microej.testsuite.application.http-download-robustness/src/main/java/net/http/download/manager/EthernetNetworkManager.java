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

/**
 * This is the Ethernet network manager.
 */
public class EthernetNetworkManager implements IEthernetNetworkManager {

	/**
	 * Logger
	 */
	private static final Logger LOGGER = Logger.getLogger(EthernetNetworkManager.class.getName());

	/**
	 * Network interface name property
	 */
	private static final String INTERFACE_NAME_PROPERTY = "eth.interface.name"; //$NON-NLS-1$

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
	 * Ethernet network interface.
	 */
	private NetworkInterface ethernet = null;

	/**
	 * Initializes the network
	 *
	 * @throws IOException
	 *             if any exception occurs when initializing the network
	 */
	private void initNetwork() throws IOException {
		LOGGER.info("Initializes the Ethernet network"); //$NON-NLS-1$

		/**
		 * Check if Ethernet interface is found
		 */
		this.ethernet = NetUtils.findInterface(this.interfaceName);
		if (this.ethernet == null) {
			throw new IOException("No Ethernet interface found"); //$NON-NLS-1$
		}

		/**
		 * Start Ethernet interface
		 */
		if (!NetworkInterfaceManager.isStarted(this.ethernet)) {
			NetworkInterfaceManager.start(this.ethernet);
		}

		/**
		 * Enable Ethernet interface
		 */
		if (!NetworkInterfaceManager.isEnabled(this.ethernet)) {
			NetworkInterfaceManager.enable(this.ethernet);
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
		IPConfiguration ethernetIPConfig;
		InetAddress[] dnsList;

		/**
		 * Perform IP configuration
		 */
		userIPConfig = new IPConfiguration();
		userIPConfig.useDHCP(true);
		userIPConfig.useStaticDNS(false);
		NetworkInterfaceManager.configure(this.ethernet, userIPConfig);

		/**
		 * Wait until an IP address is assigned
		 */
		NetUtils.waitUntilIpIsAssigned(this.interfaceName);

		/**
		 * Get IP configuration
		 */
		ethernetIPConfig = NetworkInterfaceManager.getIPConfiguration(this.ethernet);
		dnsList = ethernetIPConfig.getDNS();

		/**
		 * Print IP configuration
		 */
		LOGGER.info("My IP address: " + ethernetIPConfig.getIP() + " (assigned " //$NON-NLS-1$//$NON-NLS-2$
				+ (ethernetIPConfig.isStatic() ? MANUALLY : BY_DHCP) + ")"); //$NON-NLS-1$
		LOGGER.info("My netmask: " + ethernetIPConfig.getNetmask() + " (assigned " //$NON-NLS-1$//$NON-NLS-2$
				+ (ethernetIPConfig.isStatic() ? MANUALLY : BY_DHCP) + ")"); //$NON-NLS-1$
		LOGGER.info("My gateway: " + ethernetIPConfig.getGateway() + " (assigned " //$NON-NLS-1$ //$NON-NLS-2$
				+ (ethernetIPConfig.isStatic() ? MANUALLY : BY_DHCP) + ")"); //$NON-NLS-1$
		LOGGER.info("My DNS list (assigned " + (ethernetIPConfig.isStatic() ? MANUALLY : BY_DHCP) + "):"); //$NON-NLS-1$ //$NON-NLS-2$
		for (int i = 0; i < dnsList.length; i++) {
			LOGGER.info("    * " + dnsList[i]); //$NON-NLS-1$
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
		if (this.ethernet != null) {
			this.ethernet = null;
		}
	}

	@Override
	public void setDefault() throws IOException {
		NetworkInterfaceManager.setDefault(this.ethernet);
	}
}