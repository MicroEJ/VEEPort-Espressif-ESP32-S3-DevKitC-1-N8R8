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

import ej.ecom.mobile.MobileManager;
import ej.ecom.mobile.PDPSession;
import ej.ecom.network.IPConfiguration;
import ej.ecom.network.NetworkInterfaceManager;
import net.http.download.Config;

/**
 * This is the Mobile network manager.
 */
public class MobileNetworkManager implements IMobileNetworkManager {

	/**
	 * Logger
	 */
	private static final Logger LOGGER = Logger.getLogger(MobileNetworkManager.class.getName());

	/**
	 * Network interface name property
	 */
	private static final String INTERFACE_NAME_PROPERTY = "mobile.interface.name"; //$NON-NLS-1$

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
	 * PDP Session instance
	 */
	private PDPSession pdpSessionManager = null;

	/**
	 * Mobile network interface.
	 */
	private NetworkInterface mobile = null;

	/**
	 * Initializes the network
	 *
	 * @throws IOException
	 *             if any exception occurs when initializing the network
	 */
	private void initNetwork() throws IOException {
		LOGGER.info("Initializes the Mobile network"); //$NON-NLS-1$

		/**
		 * Check if mobile interface is found
		 */
		this.mobile = NetUtils.findInterface(this.interfaceName);
		if (this.mobile == null) {
			throw new IOException("No Mobile interface found"); //$NON-NLS-1$
		}

		/**
		 * Set the mobile configuration
		 */
		String apn = System.getProperty(Config.MOBILE_APN);
		String user = System.getProperty(Config.MOBILE_USER);
		String passphrase = System.getProperty(Config.MOBILE_PASSPHRASE);
		this.pdpSessionManager = new PDPSession(apn, user, passphrase);

		/**
		 * Start mobile interface
		 */
		if (!NetworkInterfaceManager.isStarted(this.mobile)) {
			NetworkInterfaceManager.start(this.mobile);
		}

		/**
		 * Enable mobile interface
		 */
		if (!NetworkInterfaceManager.isEnabled(this.mobile)) {
			NetworkInterfaceManager.enable(this.mobile);
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
		IPConfiguration mobileIPConfig;
		InetAddress[] dnsList;

		/**
		 * Perform IP configuration
		 */
		userIPConfig = new IPConfiguration();
		userIPConfig.useDHCP(true);
		userIPConfig.useStaticDNS(false);
		NetworkInterfaceManager.configure(this.mobile, userIPConfig);

		/**
		 * Joins the configured Access Point
		 */
		LOGGER.info("Joining Mobile Access Point"); //$NON-NLS-1$

		/**
		 * Activate the PDP Session
		 */
		this.pdpSessionManager.activate();

		/**
		 * Wait until an IP address is assigned
		 */
		NetUtils.waitUntilIpIsAssigned(this.interfaceName);

		/**
		 * Get IP configuration
		 */
		mobileIPConfig = NetworkInterfaceManager.getIPConfiguration(this.mobile);
		dnsList = mobileIPConfig.getDNS();

		/**
		 * Print IP configuration
		 */
		LOGGER.info("My IP address: " + mobileIPConfig.getIP() + " (assigned " //$NON-NLS-1$//$NON-NLS-2$
				+ (mobileIPConfig.isStatic() ? MANUALLY : BY_DHCP) + ")"); //$NON-NLS-1$
		LOGGER.info("My netmask: " + mobileIPConfig.getNetmask() + " (assigned " //$NON-NLS-1$//$NON-NLS-2$
				+ (mobileIPConfig.isStatic() ? MANUALLY : BY_DHCP) + ")"); //$NON-NLS-1$
		LOGGER.info("My gateway: " + mobileIPConfig.getGateway() + " (assigned " //$NON-NLS-1$ //$NON-NLS-2$
				+ (mobileIPConfig.isStatic() ? MANUALLY : BY_DHCP) + ")"); //$NON-NLS-1$
		LOGGER.info("My DNS list (assigned " + (mobileIPConfig.isStatic() ? MANUALLY : BY_DHCP) + "):"); //$NON-NLS-1$ //$NON-NLS-2$
		for (int i = 0; i < dnsList.length; i++) {
			LOGGER.info("    * " + dnsList[i]); //$NON-NLS-1$
		}
	}

	@Override
	public void init() throws IOException {
		MobileManager.getManager().setPinCode(System.getProperty(Config.MOBILE_PIN_CODE));
		initNetwork();
		joinNetwork();
		setDefault();
		NetUtils.updateLocalTime();
	}

	@Override
	public void deinit() throws IOException {
		if (this.pdpSessionManager != null) {
			this.pdpSessionManager.deactivate();
			this.pdpSessionManager = null;
		}

		if (this.mobile != null) {
			this.mobile = null;
		}
	}

	@Override
	public void setDefault() throws IOException {
		NetworkInterfaceManager.setDefault(this.mobile);
	}
}