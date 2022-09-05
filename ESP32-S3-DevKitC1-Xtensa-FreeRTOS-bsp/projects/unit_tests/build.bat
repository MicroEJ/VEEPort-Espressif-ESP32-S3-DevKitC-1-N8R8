@echo off
SETLOCAL ENABLEEXTENSIONS

REM BAT
REM Copyright 2022 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

REM 'build.bat' implementation for Espressif IDF.

REM 'build.bat' is responsible for producing the executable file. It will build the whole project
REM according to the sdkconfig.defaults configuration file. 
REM Output will be in %PROJECT_DIR%\Build directory.

REM Reset ERRORLEVEL between multiple run in the same shell
SET ERRORLEVEL=0

REM Get current directory
SET CURRENT_DIR=%CD%
ECHO CURRENT_DIR=%CURRENT_DIR%
CD "%~dp0"

REM Check if the project environment variables are set
CALL set_project_env.bat
IF %ERRORLEVEL% NEQ 0 (
	EXIT /B %ERRORLEVEL%
)

CALL %ESP_IDF_PATH%\export.bat
IF %ERRORLEVEL% NEQ 0 (
	EXIT /B %ERRORLEVEL%
)

CD "%ESP_PROJECT_DIR%"

IF %ERRORLEVEL% NEQ 0 (
	EXIT /B %ERRORLEVEL%
)

python "%ESP_IDF_PATH%\tools\idf.py" -B "%ESP_BUILD_DIR%" all && COPY /Y /Z /B "%ESP_BUILD_DIR%\unit_tests.elf" "%CURRENT_DIR%\application.out" && (
	SET ERRORLEVEL=0
	) || (
	SET ERRORLEVEL=1
)
IF %ERRORLEVEL% NEQ 0 (
	EXIT /B %ERRORLEVEL%
)

REM Restore current directory
CD "%CURRENT_DIR%"

IF %ERRORLEVEL% NEQ 0 (
	EXIT /B %ERRORLEVEL%
)

REM Clean exit.


