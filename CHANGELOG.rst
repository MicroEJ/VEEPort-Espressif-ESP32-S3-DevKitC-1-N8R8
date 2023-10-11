..
    Copyright 2022-2023 MicroEJ Corp. All rights reserved.
    Use of this source code is governed by a BSD-style license that can be found with this software.

===========
 Changelog
===========

----------------------
 [2.3.0] - 2023-10-04
----------------------

Fixed
=====

- Fix git patch needed for MicroEJ extension of SystemView (SYSVIEW-MicroEJ.patch).
- Fix SystemView documentation.
- Fix Exception in thread "FeatureFinalizer".
- Fix stack overflow in ECOM-WIFI worker task.
- Fix stack overflow in ASYNC-SELECT stack.
- Fix assert when running an application with ecom-wifi dependency and without net dependency.
- Fix DNS management via ecom-network foundation library.
- Fix network configuration via the launcher's "Network Settings" page.
- Fix missing Core testsuite natives.
- Fix WiFi capability (WIFI_CAPABILITY_BOTH_SIMULTANEOUS).
- Fix re-starting of DHCP server by ecom-network natives.
- Fix FS Tests with application level tracing activated or WL_SECTOR_SIZE set to 4096.
- Fix Fileystem not mounting issue.

Updated
=======

- Update MicroEJ architecture to simikou7#7.20.1.
- Update to Espressif ESP-IDF v5.0.1.
- Update bluetooth-pack to version 2.3.0.
- Update net pack to version 10.5.0.
- Update fs pack to version 6.0.4.
- Update com.microej.api#esp_idf foundation library to version 1.1.0.
- Update async_select CCO to version 2.3.2.
- Update bsp-microej-async-worker CCO to version 0.4.0.
- Update bsp-microej-main CCO to version 2.1.0.
- Update ecom-network-generic CCO to version 2.3.1.
- Update ecom-wifi-generic CCO to version 2.3.0.
- Update esp_idf CCO to version 1.2.0.
- Update fs-fatfs CCO to version 2.1.2.
- Update fs-generic CCO to version 2.1.1.
- Update net-bsd CCO to version 1.4.3.
- Update net-ssl-mbedtls CCO to version 2.1.7.
- Update osal-FreeRTOS CCO to version 1.1.0.
- Update osal-headers CCO to version 1.0.3.
- Update security-mbedtls CCO to version 1.2.0.
- Update trace-systemview CCO to version 2.2.0.
- Update mjvm-freertos CCO to version 1.4.0.
- Changed the default sdkconfig to sdkconfig_ota_systemview.
- Increased PSRAM clock to 80MHz.
- Changed WL_SECTOR_SIZE to 4096 in sdkconfig files (default configuration).
- Update PCA scripts to version 2.10.0.

Added
=====

- Add UI pack simikou7-ui-pack#13.5.1 with LED support.
- Add ecom-network-pack#1.0.0.
- Add ecom-wifi-pack#1.0.0.
- Add bluetooth-esp-idf#1.0.0 CCO.
- Add net-dns-lwip#0.1.3 CCO.
- Add net-lwip#0.2.2 CCO.
- Add idf-extra-components repository as submodule.

Removed
=======

- Remove net-addons pack.
- Remove watchdog-timer-checkpoint-freertos CCO.
- Remove watchdog-timer-generic CCO.
- Cleanup configuration folder.
- Remove build_ota_systemview scripts.
- Remove useless bdroid_buildcfg.h, interrupts.h and interrupts.c files as they are not used.
- Remove already enabled mbedTLS features from microej_mbedtls_config.h.

----------------------
 [2.2.1] - 2023-02-17
----------------------

Added
=====

- "-O3" compiler optimization flag

Fixed
=====

- fix validation projects

----------------------
 [2.2.0] - 2023-02-07
----------------------

Updated
=======

- Change flash mode to QIO
- Increase instruction cache size from 16kB to 32kB
- Increase data cache size from 32kB to 64kB
- Increase data cache line size from 32B to 64B
- Refactored validation projects
- Removed net-ssl-2_2-testsuite-openjdk dependency from SSL validation project as it is not applicable for the VEE Port
- Ignored TestKeystoreManipulation test from SSL validation project due to out of date testsuite configuration

----------------------
 [2.1.0] - 2023-01-19
----------------------

Added
=====

- VEE Port validation projects (CORE, FS, NET, SSL, SECURITY, BLUETOOTH, ESP32)

Updated
=======

- MicroEJ architecture to 7.18.1
- FS pack to 6.0.3

Fixed
=====

- Disable Mbed TLS certificate bundle
- Enable Mbed TLS DHE RSA and DHE PK, DHM_C, DES_C, Blowfish and Xtea
- Fix parameters passed to MbedTLS ``mbedtls_ecdsa_write_signature`` function
- Correctly escape CMAKE_C_FLAGS to allow configuration step to find ``sizeof(time_t)``

---------------------
 [2.0.0] - 2022-12-13
---------------------

Added
=====

- PSRAM support

Changed
=======

- Update to Espressif ESP-IDF v5.0
- Update Cmake from VERSION 3.10 to VERSION 3.16 and add "REQUIRES" and "PRIV_REQUIRES" to the idf_component_register rule
- Led driver use the new RMT driver
- Update from Mbed TLS 2.x to Mbed TLS 3.0
- EXT_RAM_ATTR is renamed to EXT_RAM_BSS_ATTR

Removed
=======

- padlock, xtea, blowfish from LLNET_SSL_ERRORS
- support for TLS 1.0, 1.1 and DTLS 1.0
- support for SSL 3.0

----------------------
 [1.0.1] - 2022-09-29
----------------------

Fixed
=====

- Fix documentation issues.
- Set pack FS to 6.0.2 to fix a resolution issue with 6.0.3.

----------------------
 [1.0.0] - 2022-09-05
----------------------

Added
=====

- Initial release of the platform.
