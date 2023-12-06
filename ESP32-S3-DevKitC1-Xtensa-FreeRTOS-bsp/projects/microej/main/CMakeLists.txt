#
# Makefile
#
# Copyright 2022-2023 MicroEJ Corp. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be found with this software.
#

if(DEFINED ENV{VALIDATION_BUILD})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVALIDATION_BUILD")
    set(srcs 
        "microej.c"
        "../validation/framework/c/embunit/embUnit/AssertImpl.c"
        "../validation/framework/c/embunit/embUnit/RepeatedTest.c"
        "../validation/framework/c/embunit/embUnit/stdImpl.c"
        "../validation/framework/c/embunit/embUnit/TestCaller.c"
        "../validation/framework/c/embunit/embUnit/TestCase.c"
        "../validation/framework/c/embunit/embUnit/TestResult.c"
        "../validation/framework/c/embunit/embUnit/TestRunner.c"
        "../validation/framework/c/embunit/embUnit/TestSuite.c"
        "../validation/tests/core/c/src/t_core_main.c"
        "../validation/tests/core/c/src/t_core_core_benchmark.c"
        "../validation/tests/core/c/src/t_core_print.c"
        "../validation/tests/core/c/src/t_core_ram.c"
        "../validation/tests/core/c/src/t_core_time_base.c"
        "../validation/tests/core/c/src/x_impl_ram_speed.c"
        "../validation/tests/core/c/src/x_ram_checks.c"
        "../validation/tests/core/c/src/x_ram_speed.c"
        "../validation/framework/c/utils/src/u_print.c"
        "../validation/framework/c/CoreMark/core_util.c"
        "../validation/framework/c/CoreMark/core_state.c"
        "../validation/framework/c/CoreMark/core_matrix.c"
        "../validation/framework/c/CoreMark/core_main.c"
        "../validation/framework/c/CoreMark/core_list_join.c"
        "../validation/port/src/time_base.c"
        "../validation/port/src/core_portme.c"
        "../validation/port/src/ram_checks.c"
        "../validation/port/src/core_benchmark.c"
        )
    
    set(include_dirs
        "../core/inc"
        "../validation/framework/c/CoreMark"
        "../validation/tests/core/c/inc"
        "../validation/port/inc"
        "../validation/framework/c/embunit/embUnit"
        "../validation/framework/c/embunit/textui"
        "../validation/framework/c/utils/inc"
        )
else()
    set(srcs 
        "microej.c"

        "../platform/lib/microejapp.S"

        "../bluetooth/src/bt_helper_read.c"
        "../bluetooth/src/bt_helper_write.c"
        "../bluetooth/src/bt_manager.c"
        "../bluetooth/src/bt_pump.c"
        "../bluetooth/src/LLBLUETOOTH_impl.c"

        "../core/src/LLBSP_esp32.c"
        "../core/src/LLDEVICE.c"
        "../core/src/LLMJVM_FreeRTOS.c"
        "../core/src/microej_time_freertos.c"
        "../core/src/microej_main.c"

        "../ecom-network/src/LLECOM_NETWORK_impl.c"
        "../ecom-network/src/ecom_network_helper_ESP32.c"

        "../ecom-wifi/src/LLECOM_WIFI_impl.c"
        "../ecom-wifi/src/WIFI_ESP32_driver.c"
        "../ecom-wifi/src/ecom_wifi_helper_ESP32.c"

        "../espressif/src/com_espressif_esp_idf_esp_ota_ops.c"
        "../espressif/src/com_espressif_esp_idf_esp_system.c"
        "../espressif/src/com_espressif_esp_idf_nvs.c"

        "../fs/src/fs_helper_fatfs.c"
        "../fs/src/LLFS_ESP32_init_spiflash.c"
        "../fs/src/LLFS_File_impl.c"
        "../fs/src/LLFS_impl.c"

        "../hal/src/LLHAL_GPIO.c"

        "../kf/src/LLKERNEL_SDRAM.c"

        "../net/src/async_select_cache.c"
        "../net/src/async_select_osal.c"
        "../net/src/async_select.c"
        "../net/src/LLNET_CHANNEL_bsd.c"
        "../net/src/LLNET_Common.c"
        "../net/src/LLNET_DATAGRAMSOCKETCHANNEL_bsd.c"
        "../net/src/LLNET_DNS_native_impl.c"

        "../net/src/LLNET_MULTICASTSOCKETCHANNEL_bsd.c"
        "../net/src/LLNET_NETWORKADDRESS_bsd.c"
        "../net/src/LLNET_NETWORKINTERFACE_lwip.c"
        "../net/src/LLNET_SOCKETCHANNEL_bsd.c"
        "../net/src/LLNET_STREAMSOCKETCHANNEL_bsd.c"
        "../net/src/lwip_util.c"

        "../security/src/LLSEC_CIPHER_impl.c"
        "../security/src/LLSEC_DIGEST_impl.c"
        "../security/src/LLSEC_KEY_FACTORY_impl.c"
        "../security/src/LLSEC_KEY_PAIR_GENERATOR_impl.c"
        "../security/src/LLSEC_MAC_impl.c"
        "../security/src/LLSEC_PRIVATE_KEY_impl.c"
        "../security/src/LLSEC_PUBLIC_KEY_impl.c"
        "../security/src/LLSEC_RANDOM_impl.c"
        "../security/src/LLSEC_RSA_CIPHER_impl.c"
        "../security/src/LLSEC_SECRET_KEY_FACTORY_impl.c"
        "../security/src/LLSEC_SECRET_KEY_impl.c"
        "../security/src/LLSEC_SIG_impl.c"
        "../security/src/LLSEC_X509_CERT_impl.c"

        "../ssl/src/LLNET_SSL_CONTEXT_impl.c"
        "../ssl/src/LLNET_SSL_ERRORS.c"
        "../ssl/src/LLNET_SSL_SOCKET_impl.c"
        "../ssl/src/LLNET_SSL_utils_mbedtls.c"
        "../ssl/src/LLNET_SSL_verifyCallback.c"

        "../uart/src/uart_switch.c"

        "../ui/src/LLUI_INPUT.c"
        "../ui/src/LLUI_LED.c"

        "../util/src/microej_allocator.c"
        "../util/src/microej_async_worker.c"
        "../util/src/osal_FreeRTOS.c"
        "../util/src/microej_pool.c"

        "../../../third_party/idf-extra-components/led_strip/src/led_strip_api.c"
        "../../../third_party/idf-extra-components/led_strip/src/led_strip_rmt_dev.c"
        "../../../third_party/idf-extra-components/led_strip/src/led_strip_rmt_encoder.c"
        )

    set(include_dirs 
        "../bluetooth/inc"
        "../core/inc"
        "../ecom-network/inc"
        "../ecom-wifi/inc"
        "../espressif/inc"
        "../fs/inc"
        "../microej-util/inc"
        "../net/inc"
        "../platform/inc"
        "../security/inc"
        "../ssl/inc"
        "../trace/inc"
        "../uart/inc"
        "${IDF_PATH}/components/freertos/FreeRTOS-Kernel/include/freertos"
        "../util/inc"
        "../../../third_party/idf-extra-components/led_strip/include"
        "../../../third_party/idf-extra-components/led_strip/interface"
        )

    if(CONFIG_APPTRACE_SV_ENABLE)
        list(APPEND srcs 
        "../trace/src/LLMJVM_MONITOR_sysview.c" 
        "../trace/src/LLTRACE_sysview.c")
        list(APPEND include_dirs 
        "../trace/inc")
    endif()

endif()

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${include_dirs})

if(DEFINED ENV{VALIDATION_BUILD})
    # Definition for the number of iterations for coremark validation
    add_compile_definitions(ITERATIONS=8000)

    # Added to ignore the compilation warnings from third party code.
    set(compile_flags 
        "-Wno-maybe-uninitialized"
        "-fno-strict-aliasing"
        "-Wno-unused-but-set-variable"
        "-O3")
else()

    # Added to ignore the casting between incompatible native function prototypes and SNI_closeFunction prototype.
    set(compile_flags 
        "-Wno-cast-function-type"
        "-O3")

    add_custom_command(TARGET ${COMPONENT_LIB} PRE_LINK
                COMMAND python ${CMAKE_CURRENT_LIST_DIR}/rename-sections.py ${CMAKE_OBJCOPY} ${CMAKE_CURRENT_LIST_DIR}/../platform/lib/microejapp.o)

    add_prebuilt_library(microejruntime "../platform/lib/microejruntime.a")
    add_prebuilt_library(microejapp "../platform/lib/microejapp.o")

    target_link_libraries(${COMPONENT_LIB} PUBLIC microejruntime microejapp)
endif()

# Added to ignore the unused label "start_sysview_logging" used to start 
# SysView logging.
set_source_files_properties(microej.c PROPERTIES COMPILE_FLAGS -Wno-unused-label)

target_compile_options(${COMPONENT_LIB} PRIVATE ${compile_flags})

set(CMAKE_EXE_LINKER_FLAGS "-Wl,--start-group" CACHE INTERNAL "-Wl,--end-group" FORCE)