cmake_minimum_required(VERSION 3.15)

project(HammingCode
        LANGUAGES CXX)

if (${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
        message(FATAL_ERROR "In-source build is forbidden")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-D DEBUG -O0 -g -Wall -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(HammingCode ./src/main.cpp)
target_include_directories(HammingCode PUBLIC ${CMAKE_SOURCE_DIR}/include)