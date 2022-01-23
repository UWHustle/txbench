cmake_minimum_required(VERSION 3.15)

project(
        txbench
        DESCRIPTION "A small benchmarking library for OLTP database systems"
        VERSION 0.1.0
)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

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

option(TXBENCH_BUILD_EXAMPLES "Build the examples" ON)

add_subdirectory(src)

if (TXBENCH_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()
