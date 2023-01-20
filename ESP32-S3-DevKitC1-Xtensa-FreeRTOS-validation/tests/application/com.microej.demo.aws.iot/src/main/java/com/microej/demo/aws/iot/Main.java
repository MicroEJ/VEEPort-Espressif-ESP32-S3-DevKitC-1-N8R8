/*
 * Java
 *
 * Copyright 2018-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */
package com.microej.demo.aws.iot;

import java.io.IOException;

import ej.aws.iot.Thing;
import ej.aws.iot.pubsub.ConnectOptions;
import ej.aws.iot.pubsub.IotCoreException;
import ej.bon.Timer;
import ej.components.dependencyinjection.ServiceLoaderFactory;

/**
 * Main class of the demonstration use the launchers, either [SIM] or [EMB] tu run it. <br>
 * It publishes a message every two seconds on a topic and prints it in the listener of this topic subscriber.
 */
@SuppressWarnings("nls")
public class Main {

	/**
	 * Example pub/sub topic.
	 */
	public static final String AWS_TOPIC_SAMPLE = "awsiot/demo/sample";

	// Network manager
	private final INetworkManager networkManager;

	// This Thing is my device
	private final Thing myThing;

	/**
	 * Constructor.
	 *
	 * @throws IOException
	 *             if an error occurs during initialization
	 */
	public Main() throws IOException {
		// Get the right network manager (SIM != EMB)
		this.networkManager = ServiceLoaderFactory.getServiceLoader().getService(INetworkManager.class);

		// Create my AWS IoT Thing (my device)
		ConnectOptions options = new ConnectOptions().setServerHost(Config.AWS_BROKER_HOST)
				.setServerPort(Config.AWS_BROKER_PORT).setThingId(Config.AWS_THING_ID)
				.setSslContext(SslContextHelper.getSslContext());

		this.myThing = new Thing(options);
	}

	/**
	 * Entry point of the application.
	 *
	 * @param args
	 *            the program arguments
	 */
	public static void main(String[] args) {
		try {
			Main main = new Main();
			main.start();
		} catch (IotCoreException | IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Starts the application.
	 *
	 * @throws IotCoreException
	 *             if an error occurs
	 * @throws IOException
	 */
	private void start() throws IotCoreException, IOException {
		// Connect the board to a wifi (credentials are to be configured in Config.java)
		this.networkManager.init();

		// Connect my AWS IoT Thing (my device) to the broker
		this.myThing.connect();

		System.out.println("[INFO] Device connected to the broker.");

		// Add a listener on the sample topic
		this.myThing.addListener(AWS_TOPIC_SAMPLE, new PublishTopicListener());

		System.out.println("[INFO] Update listener added, we're now subscribed to the topic " + AWS_TOPIC_SAMPLE);

		// Schedule a timer task that publishes sample messages to a topic every 2 seconds
		Timer samplePublishTimer = new Timer();
		samplePublishTimer.schedule(new PublishTimerTask(this.myThing), 0, 2000);

		System.out.println("[INFO] Sample data publishing timer task initialized.");
	}

}
