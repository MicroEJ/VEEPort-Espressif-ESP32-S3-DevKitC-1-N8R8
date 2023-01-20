/*
 * Java
 *
 * Copyright 2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
package net.http.download;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import net.http.download.manager.INetworkManager;

/**
 * Class handling downloading the resources from a multi-thread context.
 */
public class DownloadTask implements Runnable {

	private static final String DOWNLOAD_THREAD_NUM_PROPERTY = "download.thread.num";
	private static final String DOWNLOAD_THREAD_LOOP_PROPERTY = "download.thread.loop";
	private static final String DOWNLOAD_URL_PROPERTY = "download.resource.url";
	private static final String DOWNLOAD_CRC32_PROPERTY = "download.resource.crc32";
	private static final String LOG_FILTER_PROPERTY = "download.log.info.every";

	private static final int DOWNLOAD_THREAD_NUM = Integer.getInteger(DOWNLOAD_THREAD_NUM_PROPERTY);
	private static final int DOWNLOAD_THREAD_LOOP = Integer.getInteger(DOWNLOAD_THREAD_LOOP_PROPERTY);
	private static final String DOWNLOAD_URL = System.getProperty(DOWNLOAD_URL_PROPERTY);
	private static final String DOWNLOAD_CRC32 = System.getProperty(DOWNLOAD_CRC32_PROPERTY);
	private static final int LOG_FILTER_DEFAULT = 1;
	private static final int LOG_FILTER = Integer.getInteger(LOG_FILTER_PROPERTY, LOG_FILTER_DEFAULT);

	private static final String USER_AGENT = "Mozilla";
	private static final int HTTP_READ_TIMEOUT = 40000;
	private static final int HTTP_CONNECT_TIMEOUT = 10000;

	private static final Logger LOGGER = Logger.getLogger(DownloadTask.class.getName());

	private final INetworkManager networkManager;

	/**
	 * Static monitor used to synchronize the HTTP connection creation across
	 * threads.
	 */
	private static final Object LOCK = new Object();

	private DownloadTask(INetworkManager networkManager) {
		this.networkManager = networkManager;
	}

	@Override
	public void run() {
		String threadIdString = "[Thread " + Thread.currentThread().getId() + "] ";
		int loopCnt = 0;

		URL url;
		try {
			url = new URL(DOWNLOAD_URL);
		} catch (MalformedURLException e) {
			LOGGER.log(Level.SEVERE, threadIdString + "Invalid URL", e);
			return;
		}
		String crc = DOWNLOAD_CRC32;
		int loops = DOWNLOAD_THREAD_LOOP;

		do {
			String message = threadIdString + "Trying to dowload file (iteration = " + loopCnt + ") ...";
			log(message, loopCnt);

			try {
				// Open the HTTP connection
				HttpURLConnection httpConn = (HttpURLConnection) url.openConnection();
				httpConn.addRequestProperty("User-Agent", USER_AGENT);
				httpConn.setRequestProperty("Connection", "Keep-Alive");
				httpConn.setReadTimeout(HTTP_READ_TIMEOUT);
				httpConn.setConnectTimeout(HTTP_CONNECT_TIMEOUT);

				// Do a HTTP GET to ensure the underlying TCP socket is connected on the desired
				// network interface
				synchronized (LOCK) {
					// Set the desired network interface as default
					this.networkManager.setDefault();

					// HTTP GET will connect the TCP socket on the selected network interface
					int responseCode = httpConn.getResponseCode();
					if (responseCode != HttpURLConnection.HTTP_OK) {
						throw new IOException("HTTP ERROR:" + responseCode);
					}
				}

				// Download the resource
				HttpDownloadUtility.downloadFile(httpConn, crc);

				// Close the HTTP connection
				httpConn.disconnect();
			} catch (IOException e) {
				LOGGER.log(Level.SEVERE, threadIdString + "Exception occurred during download", e);
			}

			message = threadIdString + "File downloaded (iteration = " + loopCnt + ")";
			log(message, loopCnt);
			loopCnt++;
		} while (loopCnt < loops);
	}

	/**
	 * Creates download threads for the given {@link INetworkManager}.
	 *
	 * @param networkManager the manager of the network interface used for
	 *                       downloads.
	 * @return the list of threads.
	 */
	public static List<Thread> createThreads(INetworkManager networkManager) {
		List<Thread> threads = new ArrayList<>();
		int n = DOWNLOAD_THREAD_NUM;
		for (int i = 0; i < n; i++) {
			threads.add(new Thread(new DownloadTask(networkManager)));
		}
		return threads;
	}

	private static void log(String message, int count) {
		if ((count % LOG_FILTER) == 0) {
			LOGGER.info(message);
		} else {
			LOGGER.fine(message);
		}
	}
}
