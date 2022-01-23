cmake_minimum_required(VERSION 3.15)

project(
        txbench
        DESCRIPTION "A small benchmarking library for OLTP database systems"
        VERSION 0.1.0
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

option(TXBENCH_BUILD_EXAMPLES "Build the examples" ON)

include(FetchContent)

FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG v2.2.1
)
set(CXXOPTS_BUILD_EXAMPLES OFF)
set(CXXOPTS_BUILD_TESTS OFF)
set(CXXOPTS_ENABLE_INSTALL OFF)
set(CXXOPTS_ENABLE_WARNINGS OFF)
FetchContent_MakeAvailable(cxxopts)

macro(txbench_target_include_directories NAME)
    target_include_directories(
            ${NAME}
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include/txbench>
    )
endmacro()

add_library(
        txbench_common
        src/benchmark.cpp
        src/worker.cpp
        src/utilities/generator.cpp
        src/utilities/options.cpp
)
add_library(txbench::common ALIAS txbench_common)
txbench_target_include_directories(txbench_common)
target_link_libraries(txbench_common cxxopts)

add_library(txbench_tatp src/benchmarks/tatp.cpp)
add_library(txbench::tatp ALIAS txbench_tatp)
txbench_target_include_directories(txbench_tatp)
target_link_libraries(txbench_tatp txbench_common)

if (TXBENCH_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()
