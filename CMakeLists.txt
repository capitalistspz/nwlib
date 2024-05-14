cmake_minimum_required(VERSION 3.28)
project(nwlib)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(nw)

add_library(nwlib INTERFACE)

target_include_directories(nwlib INTERFACE .)

target_link_libraries(nwlib INTERFACE nw)