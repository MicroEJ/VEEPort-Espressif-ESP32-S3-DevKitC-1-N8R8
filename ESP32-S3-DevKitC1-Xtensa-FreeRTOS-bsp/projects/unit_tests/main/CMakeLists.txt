# Copyright 2022 MicroEJ Corp. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be found with this software.

idf_component_register(SRCS "unit_tests.c"
    "../embunit/embUnit/AssertImpl.c"
    "../embunit/embUnit/RepeatedTest.c"
    "../embunit/embUnit/stdImpl.c"
    "../embunit/embUnit/TestCaller.c"
    "../embunit/embUnit/TestCase.c"
    "../embunit/embUnit/TestResult.c"
    "../embunit/embUnit/TestRunner.c"
    "../embunit/embUnit/TestSuite.c"
    "../tests/src/t_core_main.c"
    "../tests/src/t_core_print.c"
    "../tests/src/t_core_ram.c"
    "../tests/src/t_core_time_base.c"
    "../tests/src/x_impl_ram_checks.c"
    "../tests/src/x_impl_ram_speed.c"
    "../tests/src/x_ram_checks.c"
    "../tests/src/x_ram_speed.c"
    "../utils/src/u_print.c"
    "../utils/src/u_time_base.c"
    
    INCLUDE_DIRS "../embunit/embUnit"
    "../embunit/textui"
    "../tests/inc"
    "../utils/inc"
)

set(compile_flags 
    "-fno-strict-aliasing"
    "-Wno-address"
	"-Wno-maybe-uninitialized"
	"-Wno-unused-variable"
)

target_compile_options(${COMPONENT_LIB} PRIVATE ${compile_flags})

set(CMAKE_EXE_LINKER_FLAGS "-Wl,--start-group" CACHE INTERNAL "-Wl,--end-group" FORCE)

