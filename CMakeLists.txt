cmake_minimum_required(VERSION 2.6)
project(emp C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -Og -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_library(all SHARED src/all.h)
add_executable(emp src/main.c)

target_link_libraries(emp LINK_PUBLIC all)

set(OUTPUT_DIR "./build")
