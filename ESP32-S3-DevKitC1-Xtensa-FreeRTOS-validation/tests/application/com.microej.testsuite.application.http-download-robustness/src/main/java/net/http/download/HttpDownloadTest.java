/*
 * Java
 *
 * Copyright 2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
package net.http.download;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import ej.components.dependencyinjection.ServiceLoaderFactory;
import net.http.download.manager.DefaultNetworkManager;
import net.http.download.manager.IEthernetNetworkManager;
import net.http.download.manager.IMobileNetworkManager;
import net.http.download.manager.INetworkManager;
import net.http.download.manager.IWifiNetworkManager;

/**
 * HTTP Download Test class
 */
public class HttpDownloadTest {

	private static final Logger LOGGER = Logger.getLogger(HttpDownloadTest.class.getName());

	private final INetworkManager[] networkManagers;

	private HttpDownloadTest() {
		this.networkManagers = loadNetworkManagers();
	}

	private void start() throws IOException {
		LOGGER.info("HttpDownloadTest started");

		List<Thread> downloadThreads = new ArrayList<>();

		// Connect to the networks and instantiate all download threads
		for (INetworkManager networkManager : this.networkManagers) {
			downloadThreads.addAll(init(networkManager));
		}

		// Start all download threads
		for (Thread downloadThread : downloadThreads) {
			downloadThread.start();
		}

		// Wait for all threads to finish
		for (Thread downloadThread : downloadThreads) {
			while (downloadThread.isAlive()) {
				try {
					downloadThread.join();
				} catch (InterruptedException e) {
					// do nothing
				}
			}
		}

		LOGGER.info("HttpDownloadTest finished");
	}

	/**
	 * HTTP download main entry point.
	 */
	public static void main(String[] args) {
		HttpDownloadTest httpDwTest = new HttpDownloadTest();
		boolean success = false;
		try {
			httpDwTest.start();
			success = true;
		} catch (IOException e) {
			LOGGER.log(Level.SEVERE, "Exception occurred", e);
		} finally {
			System.out.println(success ? "PASSED: 1" : "FAILED: 1 failures, 0 errors, out of 1 runs.");
		}
	}

	private static INetworkManager[] loadNetworkManagers() {
		List<INetworkManager> networkManagers = new ArrayList<>();
		networkManagers.addAll(DefaultNetworkManager.getInterfaces());
		addNetworkManagerService(networkManagers, IMobileNetworkManager.class);
		addNetworkManagerService(networkManagers, IEthernetNetworkManager.class);
		addNetworkManagerService(networkManagers, IWifiNetworkManager.class);
		return networkManagers.toArray(new INetworkManager[networkManagers.size()]);
	}

	private static void addNetworkManagerService(List<INetworkManager> networkManagers,
			Class<? extends INetworkManager> serviceInterface) {
		INetworkManager mobileManager = ServiceLoaderFactory.getServiceLoader().getService(serviceInterface);
		if (mobileManager != null) {
			networkManagers.add(mobileManager);
		}
	}

	private List<Thread> init(INetworkManager networkManager) throws IOException {
		List<Thread> threads = new ArrayList<>();
		if (networkManager != null) {
			networkManager.init();

			threads.addAll(DownloadTask.createThreads(networkManager));
		}
		return threads;
	}
}
