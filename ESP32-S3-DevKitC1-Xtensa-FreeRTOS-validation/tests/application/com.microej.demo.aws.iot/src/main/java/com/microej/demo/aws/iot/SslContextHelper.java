/*
 * Java
 *
 * Copyright 2018-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */
package com.microej.demo.aws.iot;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.UnrecoverableKeyException;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.util.Properties;
import java.util.Set;

import javax.net.ssl.KeyManager;
import javax.net.ssl.KeyManagerFactory;
import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManager;
import javax.net.ssl.TrustManagerFactory;

/**
 * This class provides functions to help creating key stores and trust stores for initializing the SSL context.
 */
@SuppressWarnings("nls")
public class SslContextHelper {

	private static final String TLS_V_1_2 = "TLSv1.2";

	// File defining the list of client certificates.
	private static final String SERVER_CERT_LIST_FILE = "/certificates/aws.iot.demo.client.certificates.list";

	// File defining the device's certificate and private key
	private static final String DEVICE_CERT_LIST_FILE = "/certificates/aws.iot.demo.device.certificates.properties";

	/**
	 * Builds an SSL context based on properties files.
	 *
	 * @return an SSL context.
	 */
	public static SSLContext getSslContext() {
		SSLContext context = null;
		try {
			context = SSLContext.getInstance(TLS_V_1_2);
			KeyManager[] keyManagers = getDefaultKeyManagers();
			TrustManager[] trustManagers = getDefaultTrustManagers();

			context.init(keyManagers, trustManagers, null);
		} catch (NoSuchAlgorithmException | KeyStoreException | CertificateException | IOException
				| UnrecoverableKeyException | KeyManagementException e) {
			e.printStackTrace();
		}

		return context;

	}

	/**
	 * Gets the trust managers, generated based on the default certificates list file :
	 * "/certificates/aws.iot.lib.client.certificates.list". It should lists the name of all the certificates files.
	 *
	 * @return an array of trust managers.
	 * @throws NoSuchAlgorithmException
	 *             if an error occurs
	 * @throws IOException
	 *             if an error occurs
	 * @throws KeyStoreException
	 *             if an error occurs
	 * @throws CertificateException
	 *             if an error occurs
	 */
	public static TrustManager[] getDefaultTrustManagers()
			throws NoSuchAlgorithmException, IOException, KeyStoreException, CertificateException {
		return getTrustManagers(SERVER_CERT_LIST_FILE);
	}

	/**
	 * Gets the trust managers, generated based on the certificates list file given in parameter. It should lists the
	 * name of all the certificates files.
	 *
	 * @param certificatesListFileName
	 *            the file listing all the certificates file names.
	 *
	 * @return an array of trust managers.
	 * @throws NoSuchAlgorithmException
	 *             if an error occurs
	 * @throws IOException
	 *             if an error occurs
	 * @throws KeyStoreException
	 *             if an error occurs
	 * @throws CertificateException
	 *             if an error occurs
	 */
	public static TrustManager[] getTrustManagers(String certificatesListFileName)
			throws NoSuchAlgorithmException, IOException, KeyStoreException, CertificateException {
		// Populate a new keystore with the certificates listed in the client
		// certificates file
		KeyStore store = getNewKeyStore();
		int certficateId = 0;
		for (String certificateFilename : loadCertificatesFilenames(certificatesListFileName)) {
			store.setCertificateEntry("myServer" + ++certficateId, generateCertificateFromFile(certificateFilename));
		}

		// Initialize the trust store and return the trust managers
		TrustManagerFactory trustManagerFactory = TrustManagerFactory.getInstance("X509");
		trustManagerFactory.init(store);
		return trustManagerFactory.getTrustManagers();
	}

	/**
	 * Gets the key managers, generated based on the default device properties file :
	 * "/certificates/aws.iot.lib.device.certificates.properties". It should define 3 properties :
	 * <p>
	 * certificate.file.name=/certificates/device/09b31f306b-certificate.pem # the certificate file name
	 * private.key.file.name=/certificates/device/09b31f306b-private.der # the file name of the private key associated
	 * to the certificate keystore.password=myKSPwd # the password of the key store
	 * <p>
	 *
	 * @return an array of key managers.
	 * @throws NoSuchAlgorithmException
	 *             if an error occurs
	 * @throws IOException
	 *             if an error occurs
	 * @throws KeyStoreException
	 *             if an error occurs
	 * @throws CertificateException
	 *             if an error occurs
	 * @throws UnrecoverableKeyException
	 *             if an error occurs
	 */
	public static KeyManager[] getDefaultKeyManagers() throws CertificateException, IOException,
			NoSuchAlgorithmException, KeyStoreException, UnrecoverableKeyException {
		String deviceCertificateFileName;
		String devicePrivateKeyFileName;
		String keyStorePassword;

		try (InputStream in = SslContextHelper.class.getResourceAsStream(DEVICE_CERT_LIST_FILE)) {
			if (in == null) {
				throw new IOException("Error during SSL context initalization, device's certificate file not found: "
						+ DEVICE_CERT_LIST_FILE);
			}

			Properties deviceCertProperties = new Properties();
			deviceCertProperties.load(in);

			deviceCertificateFileName = deviceCertProperties.getProperty("certificate.file.name");
			devicePrivateKeyFileName = deviceCertProperties.getProperty("private.key.file.name");
			keyStorePassword = deviceCertProperties.getProperty("keystore.password");
		}

		return getKeyManagers(deviceCertificateFileName, devicePrivateKeyFileName, keyStorePassword);
	}

	/**
	 * Gets the key managers.
	 *
	 * @param certificateFileName
	 *            the certificate file name.
	 * @param keyFileName
	 *            the file name of the private key associated to the certificate.
	 * @param keyStorePassword
	 *            the password of the key store.
	 *
	 * @return an array of key managers.
	 * @throws NoSuchAlgorithmException
	 *             if an error occurs
	 * @throws IOException
	 *             if an error occurs
	 * @throws KeyStoreException
	 *             if an error occurs
	 * @throws CertificateException
	 *             if an error occurs
	 * @throws UnrecoverableKeyException
	 *             if an error occurs
	 */
	public static KeyManager[] getKeyManagers(String certificateFileName, String keyFileName, String keyStorePassword)
			throws CertificateException, IOException, NoSuchAlgorithmException, KeyStoreException,
			UnrecoverableKeyException {
		// Generate the certificate and its key
		Certificate clientCert = generateCertificateFromFile(certificateFileName);
		byte[] key = loadResource(keyFileName);

		// Populate a new key store
		KeyStore store = getNewKeyStore();
		store.setKeyEntry("clientkey", key, new Certificate[] { clientCert });

		// Initialize the key managers and return them
		KeyManagerFactory km = KeyManagerFactory.getInstance(KeyManagerFactory.getDefaultAlgorithm());
		km.init(store, keyStorePassword.toCharArray());
		return km.getKeyManagers();
	}

	private static Certificate generateCertificateFromFile(String certificateFilename)
			throws IOException, CertificateException {
		Certificate cert = null;

		try (InputStream in = SslContextHelper.class.getResourceAsStream(certificateFilename)) {
			if (in == null) {
				throw new IOException("File not found: " + certificateFilename);
			}

			// Generate the server certificates
			cert = CertificateFactory.getInstance("X509").generateCertificate(in);
		}

		return cert;
	}

	private static KeyStore getNewKeyStore()
			throws NoSuchAlgorithmException, CertificateException, IOException, KeyStoreException {
		KeyStore store = KeyStore.getInstance(KeyStore.getDefaultType());
		store.load(null, null);
		return store;
	}

	/**
	 * Loads and returns the filenames of the certificates.
	 *
	 * @param fileName
	 *            the file listing the certificates.
	 *
	 * @throws IOException
	 *             on IO Error.
	 */
	private static Set<String> loadCertificatesFilenames(String fileName) throws IOException {
		InputStream certsfn = SslContextHelper.class.getResourceAsStream(fileName);
		try (InputStream certificatesFilnamesInput = certsfn) {

			if (certificatesFilnamesInput == null) {
				throw new IOException("File not found: " + fileName);
			}
			Properties properties = new Properties();
			properties.load(certificatesFilnamesInput);
			return properties.stringPropertyNames();
		}
	}

	/**
	 * Loads the content of a resource into a byte array.
	 * <p>
	 * This method uses {@link Class#getResourceAsStream(String)} to load the resource.
	 *
	 * @param resourceName
	 *            name of the resource
	 *
	 * @return an array of bytes filled-in with the content of the resource.
	 *
	 * @throws IOException
	 *             if an I/O error occurs during the stream reading.
	 *
	 * @see Class#getResourceAsStream(String)
	 */
	private static byte[] loadResource(String resourceName) throws IOException {
		try (InputStream stream = SslContextHelper.class.getResourceAsStream(resourceName)) {

			if (stream == null) {
				return null;
			}

			DataInputStream dataInputStream = new DataInputStream(stream);
			byte[] data = new byte[stream.available()];

			dataInputStream.readFully(data);

			return data;
		}
	}
}
