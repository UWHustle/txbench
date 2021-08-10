cmake_minimum_required(VERSION 3.15)
project(txbench)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-fpermissive")
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

include(FetchContent)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#if(MSVC)
#    set(CMAKE_CXX_FLAGS "/O2 /arch:AVX2 /W1 /EHsc")
#elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
#    set(CMAKE_CXX_FLAGS "-O3 -xHost")
#else()
    # clang and gcc
    set(CMAKE_CXX_FLAGS "-O3 -march=native")
#endif()

#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#find_package(Boost )
#message(STATUS "Boost version: ${Boost_VERSION}")
#

#find_package (glog)

#set(BOOST_INCLUDE_DIRS /usr/local/Cellar/boost/1.72.0_1/include)
#set(GLOG_INCLUDE_DIRS /usr/local/Cellar/glog/0.4.0/include)
#set(GFLAG_INCLUDE_DIRS /usr/local/Cellar/gflags/2.2.2/include)
#set(JEMALLOC_INCLUDE_DIRS /usr/local/Cellar/jemalloc/5.2.1_1/include)

#link_directories(/usr/local/Cellar/boost/1.72.0_1/lib/)
#link_directories(${GLOG_INCLUDE_DIRS}/../lib)

#add_compile_options(-mavx2 -O3 -mlzcnt -march=native)

# We need gcc's libatomic.so for AArch64. It's not required on x86.
find_package(GccAtomic)

FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG v2.2.1
)
set(CXXOPTS_BUILD_EXAMPLES OFF)
set(CXXOPTS_BUILD_TESTS OFF)
set(CXXOPTS_ENABLE_INSTALL OFF)
set(CXXOPTS_ENABLE_WARNINGS OFF)
FetchContent_MakeAvailable(cxxopts)

add_subdirectory(src)
