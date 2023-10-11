#
# Makefile
#
# Copyright 2022-2023 MicroEJ Corp. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be found with this software.
#

cmake_minimum_required(VERSION 3.16)

if(DEFINED ENV{VALIDATION_BUILD})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVALIDATION_BUILD")
endif()

# Set the project version
set(PROJECT_VER "1")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(microej)
