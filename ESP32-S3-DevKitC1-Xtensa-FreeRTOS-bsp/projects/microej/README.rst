.. 
    Copyright 2022 MicroEJ Corp. All rights reserved.
    Use of this source code is governed by a BSD-style license that can be found with this software.

.. |BOARD_NAME| replace:: ESP32-S3-DevKitC-1-N8R8
.. |VEEPORT| replace:: VEE Port
.. |RTOS| replace:: FreeRTOS RTOS
.. |MANUFACTURER| replace:: Espressif
.. |BSP_FULL_NAME| replace:: Espressif IoT Development Framework 
.. |BSP_SHORT_NAME| replace:: ESP-IDF

.. _中文版: ./docs/zn_CH/README_CN.rst
.. _README: ./../../../README.rst
.. _MicroEJ SystemView README: ./trace/systemview/README.rst 

================
|BOARD_NAME| BSP
================

`中文版`_

This project contains the BSP sources of the |VEEPORT| for the
|BOARD_NAME|.  The BSP project is based on |BSP_FULL_NAME| (|BSP_SHORT_NAME|)

This document does not describe how to setup the |VEEPORT|.  Please
refer to the `README`_ for that.

Build & Run Scripts
---------------------

In the directory ``projects/microej/scripts/`` are scripts that can be
used to update, build, flash and debug the BSP.  The ``.bat`` and ``.sh`` 
scripts are meant to run in a Windows and Linux environment respectively.

- The ``set_project_env`` script is responsible for setting the following variables:

  - ``ESPPORT`` The value of the COM port must be set by the user (e.g. `=COM6`
    or `==/dev/ttyS0`)
  - ``IDF_TOOLS_PATH`` The value of this environment variable is set by the |BSP_FULL_NAME|
    installation. It must be defined and pointing to the Xtensa toolchain installation for 
    the build/flash proccess to work.
  - ``ESP_PROJECT_DIR`` The value is set by the script. Represents the path to 
    the project directory (`\projects\microej`)
  - ``ESP_BUILD_DIR`` The value is set by the script. Represents the path to the 
    build directory (`\projects\microej\build`).
  - ``ESP_IDF_PATH`` The value is set by the script. Represents the path to the 
    |BSP_FULL_NAME| directory (`\sdk\esp-idf`)

- The ``RunAtFirstBSPCompile`` script is used to apply MicroEJ system view implementation
  patch. 

  The patch file ``SYSVIEW-MicroEJ.patch``, from the 
  ``projects/microej/trace/systemview/`` directory will be applied to the |BSP_FULL_NAME|
  directory (`\sdk\esp-idf`). This will add the MicroEJ custom 
  implementation for the Segger System View tracing system.

  The steps above can also be done manually by the user and are only required before 
  the first BSP compilation

- The ``build*`` scripts are used to compile and link the BSP with a
  MicroEJ Application to produce a MicroEJ Firmware
  (``application.out``) that can be flashed on a device.

  This script uses a configuration for the Mono-Sandbox firmware, and creates the 
  the ``sdkconfig`` configuration file using ``sdkconfig_ota_no_systemview``.

- The ``run*`` scripts are used to flash a MicroEJ Firmware
  (``application.out``) on a device. The variable ``ESPPORT`` must be set in 
  the ``set_project_env*`` for the script to work properly.

- The ``build_ota_systemview*`` script is used to compile and link the BSP with a MicroEJ 
  application to produce a MicroEJ Firmware (``application.out``) that can be
  flashed on a device.

  This script uses a configuration for the Mono-Sandbox firmware, and creates the 
  the ``sdkconfig`` configuration file using ``sdkconfig_ota_systemview``.
  In current configuration, applications that use the FS library
  do not work with SystemView activated.

- The ``build_no_ota_no_systemview*`` script is used to compile and link the BSP with a MicroEJ 
  application to produce a MicroEJ Firmware (``application.out``) that can be
  flashed on a device.

  This script uses a configuration for the Mono-Sandbox firmware, and creates the 
  the ``sdkconfig`` configuration file using ``sdkconfig_no_ota_no_systemview``.
  The SystemView tracing implementation is disabled and the output is
  configured as a single factory application.

- The ``SysView`` script is used to start the application, tracing and logging
  to files, (start the Segger SystemView tracing). More information about this feature
  can be found on `MicroEJ SystemView README`_ and also on the |MANUFACTURER| `official 
  documentation <https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32s3/api-guides/app_trace.html#system-behavior-analysis-with-segger-systemview>`_.

Debugging with |BOARD_NAME|
---------------------------

The entire process of debugging a |BOARD_NAME| is described `here <https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32s3/api-guides/jtag-debugging/using-debugger.html>`_.

Enable PSRAM
------------

This |VEEPORT| is configured for PSRAM chip ESP32-S3-Wroom-1/M0N8R8, if your hardware has a different PSRAM chip, the size, modes and speeds of flash and PSRAM has to be configured properly in ``sdkconfig`` based on the aforementioned documentation from |MANUFACTURER|.

The ESP32-S3 module flash and PSRAM sizes can be identified using the documentation `here <https://www.espressif.com/sites/default/files/documentation/espressif_module_packaging_information_en.pdf>`_.
The supported modes and speeds of the flash and PSRAM can be identified using the documentation `here <https://www.espressif.com/sites/default/files/documentation/esp32-s3-wroom-1_wroom-1u_datasheet_en.pdf>`_.
and `here <https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32s3/api-guides/flash_psram_config.html?highlight=psram>`_.

RAM overflow
------------

- In case you are experiencing ``iram0`` overflow, you can reduce the ``INSTRUCTION_CACHE`` size configured in ``sdkconfig`` from 32kB to 16kB with a slight reduction in PSRAM performance
- In case you are experiencing ``dram heap`` overflow, you can reduce the ``DATA_CACHE`` size configured in ``sdkconfig`` from 64kB to 32kB with a slight reduction in PSRAM performance

Using Segger SystemView
-----------------------

The entire process is described in `MicroEJ SystemView README`_
The feature can be used out of the box for the Mono-Sandbox configuration (created using the
``build`` script). 

Convert a PEM private key to DER
--------------------------------

MicroEJ requires private keys in DER format. To convert your PEM
private keys use the following command:

.. code-block:: console

	openssl.exe pkcs8 -inform PEM -in key1.pem -topk8 -outform DER -out key1.der -v1 PBE-SHA1-3DES -passout pass:<my_password>

Convert a backtrace without make monitor
----------------------------------------

The following command:

.. code-block:: console

	xtensa-esp32s3-elf-addr2line.exe -e build/microej.elf Backtrace:0x40081afb:0x3ffba7f0 0x4008d335:0x3ffba810 0x40092cae:0x3ffba830 0x4008bb0f:0x3ffba8a0

Should output the corresponding instructions at the addresses given as
the last parameters.


