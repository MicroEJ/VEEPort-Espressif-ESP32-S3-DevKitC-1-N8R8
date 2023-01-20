/*
 * Java
 *
 * Copyright 2018-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */
package com.microej.demo.aws.iot;

import ej.aws.iot.Thing;
import ej.aws.iot.pubsub.IotCoreException;
import ej.bon.TimerTask;

/**
 * Timer task that publishes data to a topic.
 */
@SuppressWarnings("nls")
public class PublishTimerTask extends TimerTask {

	// Data to be published in an example topic
	private static final String[] SAMPLE_DATA_PUBLISH_ARRAY = { "MicroEJ", "is", "a", "unique", "solution", "for",
			"building", "Internet", "of", "Things", "and", "embedded", "software", "and", "can", "now", "communicate",
			"with", "AWS IoT" };

	// This object synchronizes the publish on the broker
	private final Object publishLocker = new Object();

	// The AWS thing
	private final Thing myThing;

	private int index = 0;

	/**
	 * Initializes the timer taks.
	 *
	 * @param myThing
	 *            the AWS thing for being able to publish with it
	 */
	public PublishTimerTask(Thing myThing) {
		this.myThing = myThing;
	}

	@Override
	public void run() {
		try {
			synchronized (this.publishLocker) {
				// Publish to the topic some data from the array
				this.myThing.publish(Main.AWS_TOPIC_SAMPLE, SAMPLE_DATA_PUBLISH_ARRAY[this.index].getBytes());
				this.index = (this.index + 1) % SAMPLE_DATA_PUBLISH_ARRAY.length;
			}
		} catch (IotCoreException e) {
			System.out.println("[WARNING] an error occured while publishing");
		}
	}

}
