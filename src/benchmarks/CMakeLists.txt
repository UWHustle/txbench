add_library(txbench_tatp tatp.cpp)
target_include_directories(
        txbench_tatp
        PRIVATE
            ${CMAKE_SOURCE_DIR}/src
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include/txbench>
)
target_link_libraries(txbench_tatp txbench_benchmark)
