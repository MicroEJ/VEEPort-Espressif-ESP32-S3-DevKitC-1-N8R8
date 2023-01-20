/*
 * Java
 *
 * Copyright 2018-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */
package com.microej.demo.aws.iot;

import ej.aws.iot.pubsub.TopicListener;

/**
 * A topic listener that prints the received data.
 */
@SuppressWarnings("nls")
public class PublishTopicListener implements TopicListener {

	@Override
	public void onDataReceived(String topic, byte[] dataReceived) {
		// Here we have the topic on which the message is received and the data
		System.out.println("[INFO] Message received on topic " + topic + " => " + new String(dataReceived)); //$NON-NLS-2$
	}

}
