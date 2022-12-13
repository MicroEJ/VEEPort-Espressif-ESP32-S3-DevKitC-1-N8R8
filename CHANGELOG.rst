..
	Copyright 2022 MicroEJ Corp. All rights reserved.
	Use of this source code is governed by a BSD-style license that can be found with this software.

===========
 Changelog
===========

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
