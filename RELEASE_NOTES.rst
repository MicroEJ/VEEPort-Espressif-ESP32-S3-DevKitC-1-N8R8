..
    Copyright 2022-2024 MicroEJ Corp. All rights reserved.
    Use of this source code is governed by a BSD-style license that can be found with this software.

.. |BOARD_NAME| replace:: ESP32-S3-DevKitC-1-N8R8
.. |BOARD_REVISION| replace:: 1.0
.. |PLATFORM_NAME| replace:: ESP32-S3-DevKitC1 Platform
.. |PLATFORM_VER| replace:: 2.5.0
.. |RCP| replace:: MICROEJ SDK
.. |PLATFORM| replace:: MicroEJ Platform
.. |PLATFORMS| replace:: MicroEJ Platforms
.. |SIM| replace:: MicroEJ Simulator
.. |ARCH| replace:: MicroEJ Architecture
.. |CIDE| replace:: MICROEJ SDK
.. |RTOS| replace:: FreeRTOS RTOS
.. |MANUFACTURER| replace:: Espressif

.. _README MicroEJ BSP: ./ESP32-S3-DevKitC1-Xtensa-FreeRTOS-bsp/projects/microej/README.rst
.. _RELEASE NOTES: ./RELEASE_NOTES.rst
.. _CHANGELOG: ./CHANGELOG.rst

.. _release-notes:

========================================================
|PLATFORM| Release Notes for |MANUFACTURER| |BOARD_NAME|
========================================================

Description
===========

This is the release notes of the |PLATFORM| for |BOARD_NAME|.

Versions
========

Platform
--------

|PLATFORM_VER|

Dependencies
------------

This |PLATFORM| contains the following dependencies:

.. list-table::
   :header-rows: 1
   
   * - Dependency Name
     - Version
   * - Architecture (simikou7)
     - 7.20.1
   * - UI Pack (simikou7UI)
     - 14.0.0
   * - NET Pack
     - 10.7.0
   * - HAL Pack
     - 2.0.2
   * - BLUETOOTH Pack
     - 2.3.1
   * - FS Pack
     - 6.0.4
   * - DEVICE Pack
     - 1.1.1
   * - ECOM-NETWORK Pack
     - 1.0.0
   * - ECOM-WIFI Pack
     - 1.0.1

Please refer to the |PLATFORM| `module description file <./ESP32-S3-DevKitC1-Xtensa-FreeRTOS-configuration/module.ivy>`_ 
for more details.

Board Support Package
---------------------

- BSP provider: |MANUFACTURER| (``esp-idf``)
- BSP version: v5.0.1

Please refer to the |MANUFACTURER| ``esp-idf`` GitHub git repository
available `here
<https://github.com/espressif/esp-idf/tree/v5.0.1>`__.

Third Party Software
--------------------

Third party softwares used in BSP can be found `here
<https://github.com/espressif/esp-idf/tree/v5.0.1/components>`__. Here
is a list of the most important ones:

.. list-table::
   :widths: 3 3 3

   * - RTOS 
     - FreeRTOS
     - 10.4.3
   * - TCP/IP stack 
     - esp_lwip
     - 2.1.2
   * - Cryptographic stack 
     - Mbed TLS
     - 3.2.1
   * - File System stack 
     - FatFS
     - R0.13c
   * - Bluetooth stack 
     - BLUEDROID
     - N/A

Features
========

Graphical User Interface
------------------------

|PLATFORM| features a graphical user interface.  It includes
one user LED.

LEDs
~~~~

The board provides an RGB matrix with 3 colored LEDs (red, green,
blue).

Network
-------

|PLATFORM| features a network interface with Wi-Fi as an
underlying hardware media.  A limited number of 10 sockets could be
used for TCP connections, 10 for TCP listening (server) connections
and 10 for UDP connections. A DHCP client can be activated to retrieve
a dynamic IP address. All DNS requests are handled by LwIP.

SSL
-------

|PLATFORM| features a network secure interface. Available
secured protocols are TLS 1.2. Supported
keys and certificates formats are PKCS#5 and PKCS#12, PEM or DER
encoded.

File System
-------

|PLATFORM| features a file system interface. SPI flash is used for storage. 
Up to 2 files can be opened simultaneously.

Known issues/limitations
========================

- NET LL API cannot change the socket
  send and receive buffer sizes (``LLNET_CHANNEL_IMPL_setOption`` function),
- FS API does not support file
  write/read with offset from/to immortal arrays,
- FS API does not support file backward
  skip,
- FS performances are low due to the use of FatFS backend combined with Espressif's Wear Levelling component
  that allows a FAT filesystem to be mounted on a SPI flash memory,
- IPV6 is not supported,
- OTA is enabled only on the Mono-Sandbox Platform. It has been disabled on the Multi-Sandbox Platform in order to fit into the FLASH memory.
- On Windows, the toolchain doesn't support long paths. 
  The build directory, set with the variable ``ESP_BUILD_DIR``, can be moved closer to the root of the filesystem. 
  To override the default value create a copy of this file: ``ESP32-S3-DevKitC1-Xtensa-FreeRTOS-bsp\projects\microej\scripts\set_local_env.bat.tpl``. 
  Remove the ``.tpl`` at the end of the file name and set the variable ``ESP_BUILD_DIR``, for example with ``C:\tmp``, to prevent toolchain errors.
- Due to esp-idf limitation that doesn't provide valid cipher type for WEP networks, all WEP networks will be scanned as WEP128, regardless of their
  cipher type.

Platform Memory Layout
======================

Memory Sections
---------------

Each memory section is discribed in the GCC linker file available
`here
<https://github.com/espressif/esp-idf/tree/v5.0.1/components/esp_system/ld/esp32s3>`__

Memory Layout
-------------

.. list-table::
   :header-rows: 1
   
   * - Section Content
     - Section Source
     - Section Destination
     - Memory Type
   * - MicroEJ Application static
     - ``.bss.soar``
     - ``.bss``
     - internal RAM
   * - MicroEJ Application threads stack blocks 
     - ``.bss.vm.stacks.java``
     - ``.ext_ram.bss``
     - external PSRAM
   * - MicroEJ Core Engine internal heap 
     - ``ICETEA_HEAP``
     - ``.ext_ram.bss``
     - external PSRAM
   * - MicroEJ Application heap 
     - ``_java_heap``
     - ``.ext_ram.bss``
     - external PSRAM
   * - MicroEJ Application Immortal Heap 
     - ``_java_immortals``
     - ``.ext_ram.bss``
     - external PSRAM
   * - MicroEJ Application resources 
     - ``.rodata.resources``
     - ``.rodata``
     - external QSPI
   * - MicroEJ System Applications code and resources 
     - ``.rodata.soar.features``
     - ``.rodata``
     - external QSPI
   * - MicroEJ System Application statics 
     - ``.bss.features.installed``
     - ``.ext_ram.bss``
     - external PSRAM
   * - MicroEJ Shielded Plug data 
     - ``.shieldedplug``
     - ``.rodata``
     - external QSPI
   * - MicroEJ Application and Library code 
     - ``.text.soar``
     - ``.rodata``
     - external QSPI

For the C heap, please refer to the |MANUFACTURER| documentation
available `here
<https://docs.espressif.com/projects/esp-idf/en/v5.0.1/esp32s3/api-reference/system/heap_debug.html#heap-information>`__

Information on MicroEJ memory sections can be found `here
<./ESP32-S3-DevKitC1-Xtensa-FreeRTOS-bsp/projects/microej/main/rename-sections.py>`__.

Please also refer to the MicroEJ docs website page available `here
<https://docs.microej.com/en/latest/PlatformDeveloperGuide/coreEngine.html#link>`__
for more details.

