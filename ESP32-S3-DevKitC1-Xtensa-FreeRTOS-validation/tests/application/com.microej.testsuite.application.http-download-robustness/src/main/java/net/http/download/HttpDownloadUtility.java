/*
 * Java
 *
 * Copyright 2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
package net.http.download;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.util.logging.Logger;

/**
 * A utility that downloads a file from a URL.
 */
public class HttpDownloadUtility {

	/**
	 * Logger
	 */
	private static final Logger LOGGER = Logger.getLogger(HttpDownloadUtility.class.getName());

	/**
	 * Size of the buffer used by the download process
	 */
	private static final int BUFFER_SIZE = 4096;

	/**
	 * Percentage step on which to log the download progress
	 */
	private static final int PERCENTAGE_STEP = 10;

	/**
	 * Avoid the utility class to be instantiated.
	 */
	private HttpDownloadUtility() {

	}

	/**
	 * Downloads a file from a URL      
	 *
	 * @param httpConn
	 *            HTTP connection handle of the file to be downloaded
	 * @param crc32
	 *            CRC32 expected value for consistency check against downloaded file
	 *            CRC32
	 * @throws IOException
	 *                  
	 */
	public static void downloadFile(HttpURLConnection httpConn, String crc32) throws IOException {
		String threadId = "[Thread " + Thread.currentThread().getId() + "] ";
		CRC32 computedCrc32 = new CRC32();

		String disposition = httpConn.getHeaderField("Content-Disposition");
		String contentType = httpConn.getContentType();
		int contentLength = httpConn.getContentLength();

		int currentSize = 0;
		int pct = 0;
		int lastPrintedPct = 0;

		LOGGER.fine(threadId + "Content-Type = " + contentType);
		LOGGER.fine(threadId + "Content-Disposition = " + disposition);
		LOGGER.fine(threadId + "Content-Length = " + contentLength);

		/**
		 * Opens input stream from the HTTP connection
		 */
		try (InputStream inputStream = httpConn.getInputStream()) {
			/**
			 * Read data from the HTTP input stream
			 */
			int bytesRead = -1;
			byte[] buffer = new byte[BUFFER_SIZE];
			while ((bytesRead = inputStream.read(buffer)) != -1) {
				currentSize += bytesRead;
				pct = (PERCENTAGE_STEP * currentSize / contentLength) * PERCENTAGE_STEP;
				if (pct > lastPrintedPct) {
					LOGGER.fine(threadId + "Progress = " + pct + "%");
					lastPrintedPct = pct;
				}

				/**
				 * Add the downloaded bytes to the CRC
				 */
				computedCrc32.update(buffer, 0, bytesRead);
			}
		}

		/**
		 * CRC32 check
		 */
		String crc32HexString = Integer.toHexString(computedCrc32.getValue());
		String paddedCrc32HexString = "00000000".substring(crc32HexString.length()) + crc32HexString;
		if (crc32.equals(paddedCrc32HexString)) {
			LOGGER.fine(threadId + "CRC32 match [" + crc32 + "]");
		} else {
			throw new IOException(
					threadId + "CRC32 error, expected [" + crc32 + "], computed [" + paddedCrc32HexString + "]");
		}
	}
}