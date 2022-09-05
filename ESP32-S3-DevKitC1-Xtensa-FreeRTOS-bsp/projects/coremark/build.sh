#!/bin/bash
#
# BASH
#
# Copyright 2022 MicroEJ Corp. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be found with this software.

# 'build.sh' implementation for Espressif IDF.

# 'build.sh' is responsible for producing the executable file 
# then copying this executable file to the current directory where it has been executed to a file named 'application.out'

# Save application current directory and jump one level above scripts
CURRENT_DIRECTORY=$(pwd)
cd $(dirname "$0")

. "set_project_env.sh"

cd $ESP_PROJECT_DIR

. $IDF_PATH/export.sh
idf.py -B $ESP_BUILD_DIR all && cp $ESP_BUILD_DIR/coremark.elf $CURRENT_DIRECTORY/application.out
if [ $? -ne 0 ]; then
  exit 1
fi

# Restore application directory
cd $CURRENT_DIRECTORY/
