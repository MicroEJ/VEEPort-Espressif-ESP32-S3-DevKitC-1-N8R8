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
import java.util.Date;
import java.util.Enumeration;
import java.util.logging.Level;
import java.util.logging.Logger;

import ej.net.util.NetUtil;
import ej.net.util.NtpUtil;

/**
 * Utility class with net-related functions.
 */
public class NetUtils {

	/**
	 * Logger
	 */
	private static final Logger LOGGER = Logger.getLogger(NetUtils.class.getName());

	/**
	 * Number of retries for updating the time
	 */
	private static final int NB_RETRY_GETTIME = 3;

	/**
	 * Delay in milliseconds for the network interface check
	 */
	private static final int NETIF_CHECK_DELAY_MS = 1000;

	/**
	 * Avoid the utility class to be instantiated.
	 */
	private NetUtils() {

	}

	/**
	 * Checks if a specific network interface is found and returns it if it is found
	 *
	 * @param interfaceName
	 *            name of the network interface to lookup
	 * @return a matched interface from the global network interface list, or null
	 *         if the requested interface could not be found
	 *
	 * @throws IOException
	 *             if no network interfaces are available on the platform
	 */
	public static NetworkInterface findInterface(String interfaceName) throws IOException {
		Enumeration<NetworkInterface> eNI = null;
		NetworkInterface ni = null;

		LOGGER.info("Waiting for " + interfaceName + " to be initialized..."); //$NON-NLS-1$

		while (true) {
			/**
			 * Get all network interfaces
			 */
			eNI = NetworkInterface.getNetworkInterfaces();

			/**
			 * Check if we have a valid enumeration of network interfaces
			 */
			if (eNI != null) {
				/**
				 * For each network interface
				 */
				while (eNI.hasMoreElements()) {
					/**
					 * Get the next network interface
					 */
					ni = eNI.nextElement();

					/**
					 * Check only the required interface
					 */
					if (ni.getName().compareTo(interfaceName) == 0) {
						LOGGER.info(interfaceName + " initialized!"); //$NON-NLS-1$
						return ni;
					}
				}
			}

			/**
			 * Interface not found, sleep some time and try again
			 */
			try {
				Thread.sleep(NETIF_CHECK_DELAY_MS);
			} catch (InterruptedException e) {
				throw new IOException(e);
			}
		}
	}

	/**
	 * Wait until an IP address is assigned
	 *
	 * @param interfaceName
	 *            name of the network interface to lookup
	 *
	 * @throws IOException
	 *             on access error of the network interface
	 */
	public static void waitUntilIpIsAssigned(String interfaceName) throws IOException {
		Enumeration<NetworkInterface> eNI = null;
		NetworkInterface ni = null;
		Enumeration<InetAddress> eIA = null;
		InetAddress ia = null;

		LOGGER.info("Waiting for IP address to be assigned..."); //$NON-NLS-1$

		while (true) {
			/**
			 * Get all network interfaces
			 */
			eNI = NetworkInterface.getNetworkInterfaces();

			/**
			 * Check if we have a valid enumeration of network interfaces
			 */
			if (eNI != null) {
				/**
				 * For each network interface
				 */
				while (eNI.hasMoreElements()) {
					/**
					 * Get the next network interface
					 */
					ni = eNI.nextElement();

					/**
					 * Check only the required interface
					 */
					if (ni.getName().compareTo(interfaceName) == 0) {
						/**
						 * Get Inet address
						 */
						eIA = ni.getInetAddresses();

						/**
						 * For each Inet address
						 */
						while (eIA.hasMoreElements()) {

							/**
							 * Get the next Inet address
							 */
							ia = eIA.nextElement();

							/**
							 * Check if we have a real address assigned
							 */
							if (ia != null && !ia.isLoopbackAddress() && !NetUtil.isConnected(ia)) {
								LOGGER.info("IP address assigned!"); //$NON-NLS-1$
								return;
							}
						}
					}
				}
			}

			/**
			 * Interface not found, sleep some time and try again
			 */
			try {
				Thread.sleep(NETIF_CHECK_DELAY_MS);
			} catch (InterruptedException e) {
				throw new IOException(e);
			}
		}
	}

	/**
	 * Updates the local time
	 *
	 * @throws IOException
	 *             if any exception occurs when updating the local time via the network interface
	 */
	public static void updateLocalTime() throws IOException {
		/**
		 * NTP can fail and it is normal, as the underlying UDP protocol is connection-less, so do a few retries
		 */
		int nbTry = 0;
		boolean timeUpdated = false;
		while ((nbTry < NB_RETRY_GETTIME) && !timeUpdated) {
			nbTry++;
			try {
				NtpUtil.updateLocalTime();
				timeUpdated = true;
			} catch (IOException e) {
				if (nbTry < NB_RETRY_GETTIME) {
					LOGGER.log(Level.WARNING, "Time update failed, will retry", e);
				} else {
					LOGGER.log(Level.SEVERE, "Time update failed " + NB_RETRY_GETTIME + " times", e);
				}
			}
		}

		if (timeUpdated) {
			LOGGER.info("Time updated: " + new Date(System.currentTimeMillis()));
		}
	}
}