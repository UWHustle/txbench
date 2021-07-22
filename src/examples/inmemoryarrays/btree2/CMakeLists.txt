
add_executable(
        inmemarrays_btree2
        inmem_arrays_BTREE2.cpp
        btree2v.c
        ../inmem_arrays_TATP_db.cpp
        inmem_arrays_TATP_db_BTREE2.cpp
)

target_link_libraries(inmemarrays_btree2
PRIVATE Threads::Threads
        txbench_tatp)

target_include_directories(inmemarrays_btree2
        PRIVATE ${CMAKE_SOURCE_DIR}/src)

