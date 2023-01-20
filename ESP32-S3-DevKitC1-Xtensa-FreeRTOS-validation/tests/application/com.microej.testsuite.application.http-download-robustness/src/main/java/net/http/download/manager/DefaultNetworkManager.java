/*
 * Java
 *
 * Copyright 2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
package net.http.download.manager;

import java.io.IOException;
import java.net.NetworkInterface;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Logger;

import ej.ecom.network.NetworkInterfaceManager;

/**
 * Default implementation of {@link INetworkManager} that "manage" (no-op) a
 * pre-configured network interface.
 */
public class DefaultNetworkManager implements INetworkManager {

	private static final String NETWORK_INTERFACES_PROPERTY = "network.interface";
	private static final Logger LOGGER = Logger.getLogger(DefaultNetworkManager.class.getName());

	private final NetworkInterface netif;

	/**
	 * Instantiates a manager for the network interface with the given name.
	 *
	 * @param interfaceName the name of the network interface
	 * @throws IOException if the network interface is not available
	 */
	public DefaultNetworkManager(String interfaceName) throws IOException {
		this.netif = NetUtils.findInterface(interfaceName);
	}

	@Override
	public void init() throws IOException {
		// Do nothing: the network interface is preconfigured
	}

	@Override
	public void deinit() throws IOException {
		// Do nothing: the network interface is not owned by this manager
	}

	@Override
	public void setDefault() throws IOException {
		NetworkInterfaceManager.setDefault(this.netif);
	}

	/**
	 * Creates a manager for the network interface given by the property
	 * {@code default.interface.name}.
	 *
	 * @return the manager
	 */
	public static List<INetworkManager> getInterfaces() {
		List<INetworkManager> interfaces = new ArrayList<>();
		String names = System.getProperty(NETWORK_INTERFACES_PROPERTY);
		if (names != null) {
			int from = 0;
			int to;
			while ((to = names.indexOf(',', from)) != -1) {
				if (to > from) {
					String interfaceName = names.substring(from, to).trim();
					addInterface(interfaces, interfaceName);
				}
				from = to + 1;
			}
			String interfaceName = names.substring(from).trim();
			addInterface(interfaces, interfaceName);
		}
		return interfaces;
	}

	private static void addInterface(List<INetworkManager> interfaces, String interfaceName) {
		try {
			INetworkManager netif = new DefaultNetworkManager(interfaceName);
			interfaces.add(netif);
			LOGGER.info("Network interface \"" + interfaceName + "\" registered.");
		} catch (IOException e) {
			LOGGER.warning("Network interface \"" + interfaceName + "\" not available: ignored.");
		}
	}
}
