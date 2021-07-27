
add_executable(
        inmemarrays_thrbtree
        inmem_arrays_BTREE.cpp
        threadskv10h.c
        ../inmem_arrays_TATP_db.cpp
        inmem_arrays_TATP_db_BTREE2.cpp
)

target_link_libraries(inmemarrays_thrbtree
PRIVATE Threads::Threads
        txbench_tatp)

target_include_directories(inmemarrays_thrbtree
        PRIVATE ${CMAKE_SOURCE_DIR}/src)

