add_subdirectory(tlx_lib)

add_executable(
        inmemarrays_tlx
        inmem_arrays_TLX.cpp
        ../inmem_arrays_TATP_db.cpp
        inmem_arrays_TATP_db_TLX.cpp
)

target_link_libraries(inmemarrays_tlx
        txbench_tatp
#        tlx_lib/build/tlx/libtlx.a
        tlx
#        tlx_static
        )

target_include_directories(inmemarrays_tlx INTERFACE .)

target_include_directories(inmemarrays_tlx
        PRIVATE ${CMAKE_SOURCE_DIR}/src
        PRIVATE Threads::Threads)

#target_include_directories(inmemarrays_tlx
#        )


target_link_libraries(inmemarrays_tlx
        txbench_tatp
        glog
        )