add_library(
        txbench_benchmark
        benchmark.cpp
        worker.cpp
)
target_include_directories(
        txbench_benchmark
        PRIVATE
            ${CMAKE_SOURCE_DIR}/src
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include/txbench>
)
target_link_libraries(txbench_benchmark txbench_utilities)

add_subdirectory(benchmarks)
add_subdirectory(utilities)
