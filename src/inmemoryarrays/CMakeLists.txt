add_subdirectory(alex)
#add_subdirectory(palm)
add_subdirectory(naive)
add_subdirectory(art)
#add_subdirectory(tlxmine)
#add_subdirectory(threads_btree)
#add_subdirectory(btree2)

##############

add_executable(
        inmemarrays_alex
        index.h
        inmem_arrays_TATP_db.h inmem_arrays_TATP_db.cpp
        alex/AlexIndex.h alex/AlexIndex.cpp
        alex/inmem_arrays_ALEX.cpp
)

target_link_libraries(inmemarrays_alex
        PRIVATE Threads::Threads
        txbench_tatp
        alex_lib)

target_include_directories(inmemarrays_alex
        PRIVATE ${CMAKE_SOURCE_DIR}/src
        PRIVATE Threads::Threads)


#############

add_executable(
        inmemarrays_art
        index.h
        inmem_arrays_TATP_db.h inmem_arrays_TATP_db.cpp
        art/ARTIndex.h art/ARTIndex.cpp
        art/inmem_arrays_ART.cpp
)

target_link_libraries(inmemarrays_art
        PRIVATE Threads::Threads
        txbench_tatp
        art_lib)

target_include_directories(inmemarrays_art
        PRIVATE ${CMAKE_SOURCE_DIR}/src
        PRIVATE Threads::Threads)


#############

add_executable(
        inmemarrays_btree2
        btree2/inmem_arrays_BTREE2.cpp
        btree2/btree2v.c btree2/btree2v.h
        inmem_arrays_TATP_db.cpp
)

target_link_libraries(inmemarrays_btree2
        PRIVATE Threads::Threads
        txbench_tatp)

target_include_directories(inmemarrays_btree2
        PRIVATE ${CMAKE_SOURCE_DIR}/src)



