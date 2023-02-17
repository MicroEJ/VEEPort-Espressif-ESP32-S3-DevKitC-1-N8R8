..
	Copyright 2022-2023 MicroEJ Corp. All rights reserved.
	Use of this source code is governed by a BSD-style license that can be found with this software.

===========
 Changelog
===========

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
