add_library(
        txbench_utilities
        generator.cpp generator.h
        options.cpp options.h
)
target_link_libraries(txbench_utilities cxxopts)
