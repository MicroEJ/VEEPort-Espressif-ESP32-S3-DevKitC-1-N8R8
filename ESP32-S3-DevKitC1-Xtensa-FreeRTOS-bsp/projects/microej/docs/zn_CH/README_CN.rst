.. 
    Copyright 2022-2023 MicroEJ Corp. All rights reserved.
    Use of this source code is governed by a BSD-style license that can be found with this software.

.. |BOARD_NAME| replace:: ESP32-S3-DevKitC-1-N8R8
.. |VEEPORT| replace:: VEE Port
.. |RTOS| replace:: FreeRTOS RTOS
.. |MANUFACTURER| replace:: Espressif
.. |BSP_FULL_NAME| replace:: Espressif IoT Development Framework 
.. |BSP_SHORT_NAME| replace:: ESP-IDF

.. _README: ./../../../../../README.rst
.. _MicroEJ SystemView README: ./../../trace/systemview/README.rst

================
|BOARD_NAME| BSP
================

这个工程包含了 |BOARD_NAME| 开发板的 |VEEPORT| BSP源代码。
BSP 工程基于 |BSP_FULL_NAME| (|BSP_SHORT_NAME|) 开发。

文档不包括如何创建 |VEEPORT|, 请参考 `README`_ 来创建它。

Build & Run Scripts 脚本
------------------------

``projects/microej/scripts/`` 目录中包含可以用来更新、编译、烧写和调试BSP的脚本。``.bat`` and ``.sh``
脚本分别用于Windows和Linux环境。

- ``set_project_env`` 脚本用来设置如下变量：

  - ``ESPPORT`` COM端口的值必须由用户设置（比如：`=COM6` 或者`==/dev/ttyS0`）
  - ``IDF_TOOLS_PATH`` 该环境变量的值在安装 |BSP_FULL_NAME| 时设置。必须定义并指向 Xtensa toolchain 安装位置以确保编译和烧写正常工作。
  - ``ESP_PROJECT_DIR`` 该值由脚本设置。它表示项目工程的存放路径(`\projects\microej`)。
  - ``ESP_BUILD_DIR`` 该值由脚本设置。它表示编译结果存放路径（`\projects\microej\build`）。
  - ``ESP_IDF_PATH`` 该值由脚本设置。它表示软件模块 |BSP_FULL_NAME| 的存放路径（`\sdk\esp-idf`）。

- ``RunAtFirstBSPCompile`` 脚本用来应用MicroEJ system view补丁。

  ``projects/microej/trace/systemview/`` 路径下的补丁文件 ``SYSVIEW-MicroEJ_windows.patch`` 或者 ``SYSVIEW-MicroEJ_linux.patch`` 将会被安装到目录 |BSP_FULL_NAME| （`\sdk\esp-idf`）。这将给 Segger System View 日志系统添加 MicroEJ 自定义的实现。

  用户也可以手动执行上述操作，并且仅在第一次编译BSP之前需要。

- ``build*`` 脚本用来编译、链接 BSP 和 MicroEJ 应用以生成可以烧写到设备的 MicroEJ 固件（ ``application.out`` ）。

  这个脚本使用 Mono-Sandbox 固件配置，使用 ``sdkconfig_ota_systemview`` 创建 ``sdkconfig`` 配置文件。

- ``run*`` 脚本用来烧写MicroEJ固件(``application.out``)到设备。为保证脚本正常工作，必须正确设置
  ``set_project_env*`` 文件里的变量 ``ESPPORT`` 。

- ``build_no_ota_no_systemview*`` 脚本用来编译和链接BSP与MicroEJ应用, 并生成可以烧写的
  MicroEJ固件(``application.out``) 。

  脚本使用 ``sdkconfig_no_ota_no_systemview`` 生成 ``sdkconfig`` 配置文件，
  并使用这个配置文件生成 Mono-Sandbox 固件。SystemView 跟踪实现被禁用，并且输出也被配置为单应用程序。

- ``SysView`` 脚本用于启动应用程序、跟踪和记录日志到文件（启动 Segger SystemView 跟踪）。更多关于这个特性的信息请参考 `MicroEJ SystemView README`_ 以及 |MANUFACTURER| `官方文档 <https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32s3/api-guides/app_trace.html#system-behavior-analysis-with-segger-systemview>`_.

调试 |BOARD_NAME|
---------------------------

调试 |BOARD_NAME| 的所有流程参见  `这里 <https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32s3/api-guides/jtag-debugging/using-debugger.html>`_.

使能 PSRAM
------------

|VEEPORT| 配置PSRAM芯片为ESP32-S3-Wroom-1/M0N8R8，如果您的硬件有不同的芯片，则flash和PSRAM的大小、模式和速度必须根据上述 |MANUFACTURER| 的文档在 ``sdkconfig`` 中正确配置。

ESP32-S3模块的flash和pram大小可通过文档 `这里 <https://www.espressif.com/sites/default/files/documentation/espressif_module_packaging_information_en.pdf>`_ 进行识别。
flash和PSRAM支持的模式和速度可以通过文档 `这里 <https://www.espressif.com/sites/default/files/documentation/esp32-s3-wroom-1_wroom-1u_datasheet_en.pdf>`_
和 `这里 <https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32s3/api-guides/flash_psram_config.html?highlight=psram>`_ 来确定。

RAM 溢出
------------

- 如果您遇到 ``iram0`` 溢出，您可以将 ``sdkconfig`` 中配置的 ``INSTRUCTION CACHE`` 大小从32kB减小到16kB，这会稍微降低PSRAM性能
- 如果您遇到 ``dram heap`` 溢出，您可以将 ``sdkconfig`` 中配置的 ``DATA CACHE`` 小从64kB减少到32kB，这将稍微降低PSRAM性能

使用 Segger SystemView
-----------------------

全部流程参见 `MicroEJ SystemView README`_
对于 Mono-Sandbox 配置，使用此功能是开箱即用的（使用 ``build`` 脚本构建）。

转换PEM私钥为DER格式
--------------------------------

MicroEJ需要DER格式的私钥。转换PEM私钥请按下列命令操作:

.. code-block:: console

	openssl.exe pkcs8 -inform PEM -in key1.pem -topk8 -outform DER -out key1.der -v1 PBE-SHA1-3DES -passout pass:<my_password>

无需make monitor即可转换回溯信息
----------------------------------------

使用下面的命令:

.. code-block:: console

	xtensa-esp32s3-elf-addr2line.exe -e build/microej.elf Backtrace:0x40081afb:0x3ffba7f0 0x4008d335:0x3ffba810 0x40092cae:0x3ffba830 0x4008bb0f:0x3ffba8a0

应该可以按照最后一个参数给出的地址输出相应的指令。


