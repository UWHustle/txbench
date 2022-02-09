cmake_minimum_required(VERSION 3.15)

project(
        txbench
        DESCRIPTION "A small benchmarking library for OLTP database systems"
        VERSION 0.1.0
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

option(TXBENCH_BUILD_EXAMPLES "Build the examples" ON)

add_library(
        txbench_core
        src/benchmark.cpp include/txbench/benchmark.hpp
)
target_include_directories(
        txbench_core
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/txbench>
)

add_library(
        txbench_utilities
        src/utilities/generator.cpp src/utilities/generator.hpp
        src/utilities/sql.hpp
)
target_include_directories(
        txbench_utilities
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/txbench>
)

add_library(
        txbench_tatp
        src/benchmarks/tatp.cpp include/txbench/benchmarks/tatp.hpp
)
target_include_directories(
        txbench_tatp
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/txbench>
)
target_link_libraries(txbench_tatp txbench_core txbench_utilities)

if (TXBENCH_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()
