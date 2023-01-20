/*
 * Java
 *
 * Copyright 2018-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */
package com.microej.demo.aws.iot;

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
	public void init() throws IOException;

	/**
	 * Deinit the network.
	 *
	 * @throws IOException
	 *             if I/O error occurred
	 */
	public void deinit() throws IOException;

}
