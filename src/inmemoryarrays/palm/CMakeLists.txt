add_subdirectory(palm_lib)

add_executable(
        inmemarrays_palm
        inmem_arrays_PALM.cpp
        ../inmem_arrays_TATP_db.cpp
        inmem_arrays_TATP_db_PALM.cpp
        palm_tatp_helpers.cpp
)

target_include_directories(inmemarrays_palm
        PRIVATE ${CMAKE_SOURCE_DIR}/src
        PRIVATE Threads::Threads
        PRIVATE ${BOOST_INCLUDE_DIRS}
        PRIVATE ${GLOG_INCLUDE_DIRS}
        PRIVATE ${GFLAG_INCLUDE_DIRS}
        PRIVATE ${JEMALLOC_INCLUDE_DIRS}
        )


target_link_libraries(inmemarrays_palm
        txbench_tatp
        glog
        /usr/local/Cellar/boost/1.72.0_1/lib/libboost_atomic-mt.a
        /usr/local/Cellar/boost/1.72.0_1/lib/libboost_chrono.a
        /usr/local/Cellar/boost/1.72.0_1/lib/libboost_thread-mt.a
        )