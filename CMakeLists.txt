cmake_minimum_required(VERSION 3.14)

set (PROJECT_NAME testtask)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(udp_client STATIC ./src/udp_client.h  ./src/udp_client.cpp)
add_executable (scan_proc ./src/scan_proc.cpp)
target_link_libraries(scan_proc PRIVATE udp_client)
add_executable (udp_server ./src/udp_server.cpp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})


set(PROG_LIBRARY "${PROJECT_NAME}") 

