cmake_minimum_required(VERSION 3.20)

project(DEAD_CODE_ELIMINATION)

set(LLVM_INSTALL_DIR "" CACHE PATH "LLVM installation directory")
list(APPEND CMAKE_PREFIX_PATH "${LLVM_INSTALL_DIR}/lib/cmake/llvm/")

find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(CMAKE_CXX_STANDARD 17 CACHE STRING "")

set(CMAKE_CXX_FLAGS "-O3 ${CMAKE_CXX_FLAGS} -fno-rtti")

if (NOT LLVM_ENABLE_RTTI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

add_library(RangeAnalysis MODULE
    lib/RangeAnalysis.cpp
    lib/vSSA.cpp
    lib/DeadCodeElimination.cpp
)

target_include_directories(RangeAnalysis PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
