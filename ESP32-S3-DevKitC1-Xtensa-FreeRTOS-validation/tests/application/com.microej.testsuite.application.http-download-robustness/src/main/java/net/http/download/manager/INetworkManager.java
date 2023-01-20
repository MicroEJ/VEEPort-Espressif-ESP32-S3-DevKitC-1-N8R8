/*
 * Java
 *
 * Copyright 2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
package net.http.download.manager;

import java.io.IOException;

/**
 * Class which implements this interface allow to manage a network.
 */
public interface INetworkManager {

	/**
	 * Initialize the network.
	 *
	 * @throws IOException
	 *             if I/O error occurred
	 */
	void init() throws IOException;

	/**
	 * Deinit the network.
	 *
	 * @throws IOException
	 *             if I/O error occurred
	 */
	void deinit() throws IOException;

	/**
	 * Set the network as default interface.
	 *
	 * @throws IOException
	 *             if I/O error occurred
	 */
	void setDefault() throws IOException;

}