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

This module contains the bluetooth testsuite to full validate the ESP32-WROVER platform which integrates ESP-IDF v4.3.2.

In this document you will find the steps needed to build BT testsuite with MicroEJ SDK, view output logs (traces) and store test suite results.

The steps presented in the document assumes that the installation steps for `here <https://docs.espressif.com/projects/esp-idf/en/v4.3/esp32/get-started/index.html#installation-step-by-step>`__ are already done.

Hardware needed |BOARD_NAME|
--------------------------
To run this testsuite, two devices are required. The two devices may be two different boards. For examples, you may run the central application on an ESP32-WROVER board and the peripheral application on a DA14681 board.
Two converter serial USB 2.0 to TTL UART e.g. CP2104

Configure the |BOARD_NAME|
--------------------------

To be able to run and save the logs, you will need to setup the board for the JTAG debugging. The setup operation for the hardware and also the USB drivers configuration is described `here <https://docs.espressif.com/projects/esp-idf/en/v4.3/esp32/api-guides/jtag-debugging/configure-ft2232h-jtag.html>`__. 

Installing the |MicroEJ SDK|
----------------------------

A MicroEJ SDK is needed to properly build, run. Follow this `link <https://developer.microej.com/get-started/>`__ to install the MicroEJ SDK.

Runnig the BT testsuite
------------------------

Import M0132_Bluetooth-Testsuite project in MicroEJ SDK.

Start the Serial To Socket Transmitter in MicroEJ SDK, to properly config the laucher please follow the next stepts just if you haven't already configured:
Run Configuration->MicroEJ Tool->Execution->Target->browse to import the platform
							   ->Execution->Settings: Serial To Socket Transmitter
							   ->Configuration->Serial Options->Port and Baudrate
							   ->Apply and Run.

Build both firmwares (central and peripheral) using this platform and flash it to the two devices.

Run Configuration->MicroEJ Application->Testsuite Central->Execution->Target->browse to import the platform
									                     ->Execution->Execution->Execute on Device
									                     ->Apply and Run.
Build and run platform with **build** and **run** scripts located here ``/ESP32-WROVER-Xtensa-FreeRTOS-bsp/Projects/microej/scripts``, please make sure your **set_local_env** settings are configured accordingly to ``/P0065_ESP32-WROVER/ESP32-WROVER-Xtensa-FreeRTOS-bspProjects/microej/README.md``

Run Configuration->MicroEJ Application->Testsuite Peripheral->Execution->Target->browse to import the platform
									                        ->Execution->Execution->Execute on Device
									                        ->Apply and Run.
Build and run platform with **build** and **run** scripts located here ``/ESP32-WROVER-Xtensa-FreeRTOS-bsp/Projects/microej/scripts``,config COM port in **set_local_env** to the second device.

After the buid and run will be finished, restart the two devices at the same time and read the traces of both devices with termite terminal to make sure that the tests pass.

Related resources to the BT testsuites
---------------------------------------

This is a short review of the resources needed for the SSL testsuites:

- **MicroEJ SDK** install MicroEJ SDK.
- **Run Serial To Socket Transmitter** Launcher for serial to socket transmitter.
- **Testsuite Central** Launcher for testsuite central firmware.
- **build and run** configure COM port in set_local_env.
- **Testsuite Peripheral** Launcher for testsuite peripheral firmware.
- **build and run** configure COM port in set_local_env to the second device.
- restart the two devices at the same time and read the traces of both devices.

