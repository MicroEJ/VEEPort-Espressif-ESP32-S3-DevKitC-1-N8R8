#
# Makefile
#
# Copyright 2022-2023 MicroEJ Corp. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be found with this software.
#

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/config/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ssl/inc)

if (CMAKE_HOST_WIN32)
	set(MBEDTLS_USER_CONFIG_DIRECTIVE [[-DMBEDTLS_USER_CONFIG_FILE=\"microej_mbedtls_config.h\"]])
	set(BDROID_BUILDCFG_DIRECTIVE [[-DHAS_BDROID_BUILDCFG]])
	string(CONCAT CMAKE_C_FLAGS "${CMAKE_C_FLAGS} " "${MBEDTLS_USER_CONFIG_DIRECTIVE}")
	string(CONCAT CMAKE_C_FLAGS "${CMAKE_C_FLAGS} " "${BDROID_BUILDCFG_DIRECTIVE}")
else()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMBEDTLS_USER_CONFIG_FILE='<microej_mbedtls_config.h>' -DHAS_BDROID_BUILDCFG")
endif()
