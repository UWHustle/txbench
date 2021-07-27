add_executable(
        inmemarrays_naive
        inmem_arrays_NAIVE.cpp
        ../inmem_arrays_TATP_db.cpp
)

target_include_directories(inmemarrays_naive
        PRIVATE ${CMAKE_SOURCE_DIR}/src
        PRIVATE Threads::Threads)

target_link_libraries(inmemarrays_naive
PRIVATE Threads::Threads
        txbench_tatp
      )
