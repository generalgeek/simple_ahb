cmake_minimum_required(VERSION 3.15)
project(SimpleAMBA LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_SYSTEM_NAME LINUX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

# compile systemc2.3.3
# set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build SystemC as a static library")
set(SYSTEMC_DIR ${CMAKE_SOURCE_DIR}/3rdparty/systemc)
set(SYSTEMC_INCLUDE_DIR ${SYSTEMC_DIR}/src)
set(SYSTEMC_BUILD_DIR ${CMAKE_BINARY_DIR}/systemc_build)
add_subdirectory(${SYSTEMC_DIR} ${SYSTEMC_BUILD_DIR})
link_directories(${SYSTEMC_BUILD_DIR}/src)
include_directories(${SYSTEMC_INCLUDE_DIR})
set(SYSTEMC_LIB systemc)

# compile quill-7.5.0
set(QUILL_DIR ${CMAKE_SOURCE_DIR}/3rdparty/quill)
set(QUILL_INCLUDE_DIR ${QUILL_DIR}/include)
set(QUILL_BUILD_DIR ${CMAKE_BINARY_DIR}/quill_build)
add_subdirectory(${QUILL_DIR} ${QUILL_BUILD_DIR})
# include_directories(${QUILL_INCLUDE_DIR})
set(QUILL_LIB quill)

set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/inc)
include_directories(${PROJECT_INCLUDE_DIR})

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp)
file(GLOB MAIN src/test_simple_cpu.cpp)

add_executable(SimpleAMBA ${PROJECT_SOURCES} ${MAIN})
target_link_libraries(SimpleAMBA ${SYSTEMC_LIB} ${QUILL_LIB})