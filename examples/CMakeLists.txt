add_executable(tatp_stdout tatp_stdout.cpp)
target_include_directories(tatp_stdout PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tatp_stdout txbench_tatp)
