.. image:: https://shields.microej.com/endpoint?url=https://repository.microej.com/packages/badges/sdk_5.6.json
   :alt: sdk_5.6 badge
   :align: left

.. image:: https://shields.microej.com/endpoint?url=https://repository.microej.com/packages/badges/arch_7.18.json
   :alt: arch_7.18 badge
   :align: left
..
    Copyright 2022-2023 MicroEJ Corp. All rights reserved.
    Use of this source code is governed by a BSD-style license that can be found with this software.

.. |BOARD_NAME| replace:: ESP32-S3-DevKitC-1-N8R8
.. |BOARD_REVISION| replace:: 1.0
.. |RCP| replace:: MICROEJ SDK
.. |VEEPORT| replace:: VEE Port
.. |SIM| replace:: Simulator
.. |CIDE| replace:: MICROEJ SDK
.. |RTOS| replace:: FreeRTOS RTOS
.. |MANUFACTURER| replace:: Espressif
.. |BSP_FULL_NAME| replace:: Espressif IoT Development Framework 
.. |BSP_SHORT_NAME| replace:: ESP-IDF

.. _README BSP: ./../../ESP32-S3-DevKitC1-Xtensa-FreeRTOS-bsp/projects/microej/docs/zn_CH/README.rst
.. _RELEASE NOTES: ./../../RELEASE_NOTES.rst
.. _CHANGELOG: ./../../CHANGELOG.rst

==========================================
|MANUFACTURER| |BOARD_NAME| 的 |VEEPORT|
==========================================

本项目用于为 |BOARD_NAME| 开发板构建 |VEEPORT| 。


.. image:: ../../images/ESP32-S3-DevKitC1.jpg


使用以下命令克隆代码仓库：

.. code-block:: sh

   git clone --recursive https://github.com/MicroEJ/Platform-Espressif-ESP32-S3-DevKitC-1-N8R8

务必添加 ``--recursive`` 选项。 如果您没有使用上面的选项克隆了此代码仓库，请运行另一个命令以获取所有子模块：

.. code-block:: sh

   git submodule update --init --recursive

相关文件
=============

本目录同时包含以下文件:

* `CHANGELOG`_ 用来跟踪 |BOARD_NAME| 的 |VEEPORT| 改动。
* `RELEASE NOTES`_ 会列出:

  - 支持的硬件,
  - 已知问题和限制,
  - 开发环境,
  - 依赖代码列表及版本。

* `README BSP`_ 建议用户熟悉 |MANUFACTURER| SDK/BSP 以及创建自定义构建过程等高级用法。

开发板技术规格
==============================

.. list-table::

   * - Name
     - |BOARD_NAME|
   * - Revision
     - |BOARD_REVISION|
   * - MCU part number
     - ESP32-S3-WROOM-1/1U
   * - MCU revision
     - N/A
   * - MCU architecture
     - Xtensa LX7
   * - MCU max clock frequency
     - 240 MHz
   * - Internal ROM size
     - 384 KB
   * - Internal RAM size
     - 512 KB
   * - External flash size
     - 8 MB
   * - External RAM size
     - 8 MB
   * - Power supply
     - USB, External 5V

下面是 |BOARD_NAME| 常用文档一览表链接:

- 开发板文档 `here <https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32s3/hw-reference/esp32s3/user-guide-devkitc-1.html#hardware-reference>`__
- |MANUFACTURER| 开发版入门指南 `here <https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32s3/get-started/index.html#introduction>`__
- 开发板原理图 `here <https://dl.espressif.com/dl/SCH_ESP32-S3-DEVKITC-1_V1_20210312C.pdf>`__
- 处理器技术参考手册 `here <https://www.espressif.com/sites/default/files/documentation/esp32-s3_technical_reference_manual_en.pdf>`__
- 处理器数据手册 `here <https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf>`__
- 处理器勘误表 `here <https://www.espressif.com/sites/default/files/documentation/esp32-s3_errata_en.pdf>`__

|VEEPORT| 规格
=======================

Architecture 版本号 ``7.20.1``.

|VEEPORT| 提供下面的基础库:

.. list-table::
   :header-rows: 1

   * - Foundation Library
     - Version
   * - BLUETOOTH
     - 2.2
   * - BON
     - 1.4
   * - DEVICE
     - 1.1
   * - DRAWING
     - 1.0
   * - ECOM-NETWORK
     - 2.1
   * - ECOM-WIFI
     - 2.3
   * - ESP_IDF
     - 1.1
   * - FS
     - 2.1
   * - HAL
     - 1.0
   * - KF
     - 1.6
   * - MICROUI
     - 3.2
   * - NET
     - 1.1
   * - NLS
     - 2.0
   * - SECURITY
     - 1.4
   * - SNI
     - 1.4
   * - SSL
     - 2.2
   * - TRACE
     - 1.1
   * - WATCHDOG
     - 1.0

|VEEPORT| 可以分为:

- a Mono-Sandbox |VEEPORT| (默认)
- a Multi-Sandbox |VEEPORT|

需求列表
============

- Windows 10 或 Linux (Ubuntu LTS 20.04测试通过)
- 可以连接到 `MicroEJ Central Repository <https://developer.microej.com/central-repository/>`_ 的网络
- |RCP| 发行版 ``21.11`` 或者更高,  `下载 <https://developer.microej.com/get-started/>`_
- |BOARD_NAME| 开发板

BSP Setup 设置
==============

按照 ``Manual Installation`` `here
<https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32s3/get-started/index.html#installation-step-by-step>`__ 的说明安装 |MANUFACTURER| 工具链。
使用 ``Universal online installer`` ，并配置 |BSP_FULL_NAME| (|BSP_SHORT_NAME|)
路径使其指向 ``ESP32-S3-DevKitC1-Xtensa-FreeRTOS-bsp/sdk/esp-idf``。

请注意上述文档中描述的安装路径的限制（例如，不能有空格、括号...）。

请参考 |MANUFACTURER| 文档  `here <https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32s3/api-guides/tools/idf-tools.html#list-of-idf-tools>`__
获得关于 |MANUFACTURER| 工具更详细的介绍。

Windows Toolchain 工具链
------------------------

- C/C++ 工具链: xtensa-esp32s3-elf-gcc
- C/C++ 工具链版本: 11.2.0 (crosstool-NG esp-2022r1)
- C/C++ 工具链下载链接 64-bit Windows: https://github.com/espressif/crosstool-NG/releases/download/esp-2022r1/xtensa-esp32s3-elf-gcc11_2_0-esp-2022r1-win64.zip
- C/C++ 工具链下载链接 32-bit Windows: https://github.com/espressif/crosstool-NG/releases/download/esp-2022r1/xtensa-esp32s3-elf-gcc11_2_0-esp-2022r1-win32.zip

请参考 |MANUFACTURER| 文档 `here
<https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32s3/get-started/windows-setup.html>`__
获得详细说明。

Linux Toolchain 工具链
---------------------

- C/C++ 工具链: xtensa-esp32s3-elf-gcc
- C/C++ 工具链版本: 11.2.0 (crosstool-NG esp-2022r1)
- C/C++ 工具链下载链接 64-bit Linux: https://github.com/espressif/crosstool-NG/releases/download/esp-2022r1/xtensa-esp32s3-elf-gcc11_2_0-esp-2022r1-linux-amd64.tar.xz
- C/C++ 工具链下载链接 32-bit Linux: https://github.com/espressif/crosstool-NG/releases/download/esp-2022r1/xtensa-esp32s3-elf-gcc11_2_0-esp-2022r1-linux-i686.tar.xz

请参考 |MANUFACTURER| 文档 `here
<https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32s3/get-started/linux-macos-setup.html>`__
获得详细说明。

BSP Compilation 编译
--------------------

在第一次构建 BSP 之前，确保您已经克隆了代码仓库及其所有子模块，并打上 MicroEJ Segger SystemView 补丁。
这可以通过运行位于 ``xxx/ESP32-S3-DevKitC1-Xtensa-FreeRTOS-bsp/projects/microej/scripts`` 中的脚本 ``RunAtFirstBSPCompile.*`` 实现。
有关详细信息，请参阅 `README BSP`_。


|VEEPORT| 提供了预编译的 Mono-Sandbox 应用。 通过编译BSP工程，可以验证BSP安装，并且构建 |BOARD_NAME| 固件。

若要编译 ``ESP32-S3-DevKitC1-Xtensa-FreeRTOS-bsp`` 工程,  打开
命令行终端 并输入下列命令:

**On Windows:**

.. code-block:: sh

      $ cd "xxx/ESP32-S3-DevKitC1-Xtensa-FreeRTOS-bsp/projects/microej/scripts"
      $ build.bat 

**On Linux / macOS:**

.. code-block:: sh

      $ cd "xxx/ESP32-S3-DevKitC1-Xtensa-FreeRTOS-bsp/projects/microej/scripts"
      $ build.sh 

BSP工程编译开始后， 请等待最终消息输出:

.. code-block::

      Project build complete. To flash, run this command:


如果要自定义编译脚本,请参考  `README BSP`_ 。

Board Setup 开发板设置
=====================

请参考 |MANUFACTURER| 文档 `here
<https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32s3/hw-reference/esp32s3/user-guide-devkitc-1.html>`__
获得更多信息。

Power Supply 供电
-----------------

开发板可以用USB或者外接5V供电。

请参考 文档 |MANUFACTURER| 请参考 Espressif 文档 `here
<https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32s3/hw-reference/esp32s3/user-guide-devkitc-1.html#getting-started>`__
获得更多信息。

Programming 烧写
----------------

|BOARD_NAME| 开发板可以使用 |MANUFACTURER| bootloader 烧写。
 请按下列步骤操作:

- 将开发板上标识为UART的USB接口连接到计算机
- 找到新出现的COM口
- 通过复制模板 ``ESP32-S3-DevKitC1-Xtensa-FreeRTOS-bsp/projects/microej/scripts/set_local_env.xxx.tpl`` 创建 ``ESP32-S3-DevKitC1-Xtensa-FreeRTOS-bsp/projects/microej/scripts/set_local_env.xxx`` 脚本

  ( Windows系统``xxx`` 为 ``bat`` ， Linux / macOS 系统为 ``sh`` ).
- 将 ``ESPPORT`` 变量更新成上一步新出现的COM口
- 打开命令行终端输入下面的命令:

**On Windows:**

.. code-block:: sh

      $ cd "xxx/ESP32-S3-DevKitC1-Xtensa-FreeRTOS-bsp/projects/microej/scripts"
      $ run.bat 

**On Linux / macOS:**

.. code-block:: sh

      $ cd "xxx/ESP32-S3-DevKitC1-Xtensa-FreeRTOS-bsp/projects/microej/scripts"
      $ run.sh 

固件烧写开始后，请等待下面的消息:

.. code-block::

    Leaving...
    Hard resetting via RTS pin...
    Done

|MANUFACTURER| 编译和烧写也可参考文档 `here
<https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32s3/get-started/index.html#step-8-build-the-project>`__
获得更多信息。

Logs Output 日志输出
-------------------

|VEEPORT| 使用 |BOARD_NAME| USB 虚拟 UART。在使用USB连接开发板到计算
机时, 会自动加载一个COM口。 开发板所有的日志都通过这个COM口输出。



COM口使用下面的参数:

.. list-table::
   :widths: 3 2

   * - Baudrate
     - 115200
   * - Data bits bits
     - 8
   * - Parity bits
     - None
   * - Stop bits
     - 1
   * - Flow control
     - None

烧写后，预编译的应用会从UART口输出 ``Hello World`` 。

如果运行测试集，日志必须重定向到另一个串口. 具体请参考 `Testsuite Configuration`_
里的详细解释说明.


请参考 |MANUFACTURER| 文档 `here
<https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32s3/get-started/establish-serial-connection.html#>`__
获得更多信息。

Debugging 调试
--------------

通过USB接口实现JTAG调试.

请参考 `README BSP`_ debugging章节获取更多详细信息.


|VEEPORT| Setup 配置
===================

|VEEPORT| Import 导入
--------------------

在 |RCP| Workspace 里导入工程:

- ``File`` > ``Import`` > ``Existing Projects into Workspace`` >
  ``Next``
- 在 ``Select root directory`` 中选择克隆好的工程。
- 点击 ``Finish``

在 |RCP| 里可以看到, 选择的示例被导入为几个以给定名称为前缀的工程:

- ``ESP32-S3-DevKitC1-Xtensa-FreeRTOS-configuration``: 包含 |VEEPORT| 配置说明，
  一些模块的配置文件放在指定的子目录下/或者使用配置文件
  (``.properties`` and / or ``.xml``).

- ``ESP32-S3-DevKitC1-Xtensa-FreeRTOS-bsp``: 包含 |BOARD_NAME| 开发板上已经
  测试通过的BSP软件工程文件, 一个 |CIDE| 工程, 一个移植好在 |RTOS| 上的
  MicroEJ 核心(包括扩展模块) 和 |BOARD_NAME| 开发板的支持包。

- ``ESP32-S3-DevKitC1-Xtensa-FreeRTOS-fp``: 包含用于 |SIM| 的开发板配置。
  这个工程在每次 |VEEPORT| 重新生成的时候更新。

- ``ESP32S3DevKitC1-Platform-GNUv112_xtensa-esp32s3-{version}``: |VEEPORT| 重新生成后，这个目录会产生 |RCP| |VEEPORT| 工程文件。

默认情况, |VEEPORT| 配置为 Mono-Sandbox 评估模式。如果需要配置使用
Multi-Sandbox, 需要使用 ``build_no_ota_no_systemview`` 脚本 (请参考 `RELEASE
NOTES`_ 限制依赖章节获取更多信息)。

|VEEPORT| Build 构建
--------------------

要生成 |VEEPORT|, 请按下列步骤执行:


- 在 |RCP| 工作空间里右键点击 ``ESP32-S3-DevKitC1-Xtensa-FreeRTOS-configuration`` 工程

- 点击 ``Build Module``

生成过程随后开始。 这个过程将会持续几分钟。第一次 |VEEPORT| 构建会从MicroEJ Central
Repository下载相关模块。你可以在 |RCP| console里看到生成过程的日志。

请等待最终完成消息:

.. code-block::

                          BUILD SUCCESSFUL


最终 |BOARD_NAME| 开发板的 |VEEPORT| 会全部生成以供使用。

|RCP| 里的 ``ESP32S3DevKitC1-Platform-GNUv112_xtensa-esp32s3-{version}`` |VEEPORT| 工程不应该有任何报错。

请参考
https://docs.microej.com/en/latest/ApplicationDeveloperGuide/standaloneApplication.html
获取更多如何生成MicroEJ Standalone应用的信息.

构建应用程序需要评估版本的许可证。 请参考
https://docs.microej.com/en/latest/overview/licenses.html#evaluation-license
了解如何申请并激活许可证。

Test Suite Configuration 测试集配置
==================================

在 |BOARD_NAME| 开发板上运行测试集需要将标准输出重新定向到专用的UART上。
必须配置 ``config.properties`` 里的 ``microej.testsuite.properties.debug.traces.uart`` 。

这个属性重定向UART到不同的GPIO端口。 将FTDI USB线连接到J1的D4和GND。

.. image:: ../../images/ESP32-S3-DevKitC1_Test_Connections.jpg

在 ``config.properties`` 文件里, 属性 ``target.platform.dir`` 必须设置成VEE port的绝对路径。
比如
``C:/ESP32S3DevKitC1-Platform-GNUv112_xtensa-esp32s3-{version}/source``。

Troubleshooting 故障排除
=======================

无法通过VirtualBox中运行的Linux烧写
---------------------------------------------------------------------------
烧写时按下单板上的 "boot" 按键。

Files not found during the build 编译过程中文件未找到
--------------------------------------------------

找不到文件可能是源于路径过长。请参考 `RELEASE NOTES`_ 里的已知问题和限制章节来寻求应对方法。
