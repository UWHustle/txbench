add_subdirectory(alex)
#add_subdirectory(palm)
#add_subdirectory(naive)
add_subdirectory(art)
#add_subdirectory(tlxmine)
#add_subdirectory(threads_btree)
#add_subdirectory(btree2)

##############

#add_executable(
#        inmemarrays_alex
#        Index.h
#        inmem_arrays_TATP_db.h inmem_arrays_TATP_db.cpp
#        alex/AlexIndex.h alex/AlexIndex.cpp
#        alex/inmem_arrays_ALEX.cpp
#)
#
#target_link_libraries(inmemarrays_alex
#        PRIVATE Threads::Threads
#        txbench_tatp
#        alex_lib)
#
#target_include_directories(inmemarrays_alex
#        PRIVATE ${CMAKE_SOURCE_DIR}/src
#        PRIVATE Threads::Threads)
#
#target_compile_definitions(inmemarrays_alex PRIVATE lockenabled=1)

#############

#add_executable(
#        inmemarrays_art
#        Index.h
#        inmem_arrays_TATP_db.h inmem_arrays_TATP_db.cpp
#        art/ARTIndex.h art/ARTIndex.cpp
#        art/inmem_arrays_ART.cpp
#)
#
#target_link_libraries(inmemarrays_art
#        PRIVATE Threads::Threads
#        txbench_tatp
#        art_lib)
#
#target_include_directories(inmemarrays_art
#        PRIVATE ${CMAKE_SOURCE_DIR}/src
#        PRIVATE Threads::Threads)
#
#target_compile_definitions(inmemarrays_art PRIVATE lockenabled=1)
#############

#add_executable(
#        inmemarrays_btree2
#        Index.h
#        inmem_arrays_TATP_db.h inmem_arrays_TATP_db.cpp
#        btree2/Btree2Index.h btree2/Btree2Index.cpp
#        btree2/inmem_arrays_BTREE2.cpp
#        btree2/btree2v.c btree2/btree2v.h
#)
#
#target_link_libraries(inmemarrays_btree2
#        PRIVATE Threads::Threads
#        txbench_tatp)
#
#target_include_directories(inmemarrays_btree2
#        PRIVATE ${CMAKE_SOURCE_DIR}/src)
#
#target_compile_definitions(inmemarrays_btree2 PRIVATE lockenabled=1)
#############

#add_executable(
#        inmemarrays_threadskv
#        Index.h
#        inmem_arrays_TATP_db.h inmem_arrays_TATP_db.cpp
#        threadskv/threadskv10.h threadskv/threadskv10.c
#        threadskv/inmem_arrays_THREADSKV.cpp
#)
#
#target_link_libraries(inmemarrays_threadskv
#        PRIVATE Threads::Threads
#        txbench_tatp)
#
#target_include_directories(inmemarrays_threadskv
#        PRIVATE ${CMAKE_SOURCE_DIR}/src)
#
#target_compile_definitions(inmemarrays_threadskv PRIVATE lockenabled=1)

#############

#add_executable(
#        inmemarrays_bw
#        Index.h
#        inmem_arrays_TATP_db.h inmem_arrays_TATP_db.cpp
#        bw/BWIndex.h bw/BWIndex.cpp
#        bw/BwTree/src/bwtree.h bw/BwTree/src/bwtree.cpp
#        bw/inmem_arrays_BW.cpp
#)
#
#target_link_libraries(inmemarrays_bw
#        PRIVATE Threads::Threads
#        PRIVATE ${GCCLIBATOMIC_LIBRARY}
#        txbench_tatp)
#
#target_include_directories(inmemarrays_bw
#        PRIVATE ${CMAKE_SOURCE_DIR}/src)
#
#target_compile_definitions(inmemarrays_bw PRIVATE lockenabled=0)

##############

add_executable(
        inmemarrays_artolc
        Index.h
        inmem_arrays_TATP_db.h inmem_arrays_TATP_db.cpp
        artsync/ARTSynchronized/OptimisticLockCoupling/Tree.h
        artsync/ARTSynchronized/OptimisticLockCoupling/Tree.cpp
        artsync/ARTOLCIndex.h artsync/ARTOLCIndex.cpp
        artsync/inmem_arrays_ARTOLC.cpp
)

target_link_libraries(inmemarrays_artolc
        PRIVATE Threads::Threads
        txbench_tatp
        ${TbbLib} ${JemallocLib})

target_include_directories(inmemarrays_artolc
        PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_definitions(inmemarrays_artolc PRIVATE lockenabled=0)
