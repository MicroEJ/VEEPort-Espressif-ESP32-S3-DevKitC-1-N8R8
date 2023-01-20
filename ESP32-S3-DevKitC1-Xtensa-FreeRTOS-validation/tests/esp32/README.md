..
	Copyright 2019-2022 MicroEJ Corp. All rights reserved.
	This library is provided in source code for use, modification and test, subject to license terms.
	Any modification of the source code will break MicroEJ Corp. warranties on the whole library.

.. |BOARD_NAME| replace:: ESP-WROVER-KIT V4.1
.. |BOARD_REVISION| replace:: 4.1
.. |PLATFORM_VER| replace:: 2.0.0
.. |RCP| replace:: MICROEJ SDK
.. |PLATFORM| replace:: MicroEJ Platform
.. |PLATFORMS| replace:: MicroEJ Platforms
.. |SIM| replace:: MicroEJ Simulator
.. |ARCH| replace:: MicroEJ Architecture
.. |CIDE| replace:: MICROEJ SDK
.. |RTOS| replace:: FreeRTOS RTOS
.. |MANUFACTURER| replace:: Espressif

.. _中文版: ./docs/zn_CH/README_CN.rst
.. _README: ./../../../README.rst
.. _RELEASE NOTES: ./../../../RELEASE_NOTES.rst
.. _CHANGELOG: ./../../../CHANGELOG.rst
.. _README MicroEJ BSP: ./../../README.rst

================
|BOARD_NAME| BSP
================

`中文版`_

This module contains the OTA testsuite to full validate the EESP32-WROVER platform which integrates ESP-IDF v4.3.2.

In this document you will find the steps needed to build OTA testsuite with MicroEJ SDK, view output logs (traces) and store test suite results.

The steps presented in the document assumes that the installation steps for `here <https://docs.espressif.com/projects/esp-idf/en/v4.3/esp32/get-started/index.html#installation-step-by-step>`__ are already done.

Configure the |BOARD_NAME|
--------------------------

To be able to run and save the logs, you will need to setup the board for the JTAG debugging. The setup operation for the hardware and also the USB drivers configuration is described `here <https://docs.espressif.com/projects/esp-idf/en/v4.3/esp32/api-guides/jtag-debugging/configure-ft2232h-jtag.html>`__. 

Installing the |MicroEJ SDK|
----------------------------

A MicroEJ SDK is needed to properly build, run, view and save the output logs. Follow this `link <https://developer.microej.com/get-started/>`__ to install the MicroEJ SDK.

Configure OTA testsuite
-----------------------

Import ESP32-WROVER/esp32 project in MicroEJ SDK.

In config.properties file set the ``target.platform.dir``.

Runnig the OTA testsuite
------------------------

Start the Serial To Socket Transmitter in MicroEJ SDK, to properly config the laucher please follow the next stepts just if you haven't already configured:
Run Configuration->MicroEJ Tool->Execution->Target->browse to import the platform
							   ->Execution->Settings: Serial To Socket Transmitter
							   ->Configuration->Serial Options->Port and Baudrate
							   ->Apply and Run.

To run FS testsuite simply right click on ESP32-WROVER-OTA and Build Module, which will invoke **build** and **run** scripts, please make sure your **set_local_env** settings are configured accordingly to ``/ESP32-WROVER-Xtensa-FreeRTOS-bspProjects/microej/README.md``

In config.properties file, by configuring the ``test.run.includes.pattern`` variable will run all the test suite or just one specific test.

After the buid and run will be finished, you will find test suite results in file located ``/ESP32-WROVER/esp32/target~/test/html/test/junit-noframes.html``.

Related resources to the OTA testsuites
---------------------------------------

This is a short review of the resources needed for the OTA testsuites:

- **MicroEJ SDK** install MicroEJ SDK.
- **config.properties** This file can be found in the ``/ESP32-WROVER/esp32`` folder.
- **Run Serial To Socket Transmitter** Launcher for serial to socket transmitter.
- **junit-noframes.html** test suite results.

