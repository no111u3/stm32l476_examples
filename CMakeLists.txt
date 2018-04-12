cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

project (examples VERSION 0.0.0 LANGUAGES CXX)

set(VENDOR stmicro)
set(DEVICE stm32l476)
set(BOARD stm32l476-disco)

include(cmake/compiler.cmake)

# libraries
add_subdirectory(libs/lp_bm)
add_subdirectory(libs/lp_cc_core)
add_subdirectory(libs/lp_cc_lib)
add_subdirectory(libs/lp_devices)

# examples
add_subdirectory(examples/empty)

setup_custom_compiler_flags()
setup_custom_linker_flags(${LINKER_SCRIPT} examples)
