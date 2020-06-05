cmake_minimum_required(VERSION 3.3)
project(Praktikum_PLARR)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Add extra Find modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# Subfolders
add_subdirectory(Exercise01)
add_subdirectory(Exercise02)
add_subdirectory(Exercise03)
add_subdirectory(Exercise04)
add_subdirectory(Exercise05)
add_subdirectory(Exercise06/c++)

