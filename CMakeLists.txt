cmake_minimum_required (VERSION 2.6)

project (Hardt)

set (Hardt_VERSION_MAJOR 1)
set (Hardt_VERSION_MINOR 2)
set (Hardt_VERSION_BUILD 0)
set (Hardt_VERSION Hardt_VERSION_MAJOR + "." + HARDT_VERSION_MINOR + "." + HARDT_VERSION_BUILD)

set(CMAKE_CXX_COMPILER "clang++")

configure_file (
  "${PROJECT_SOURCE_DIR}/src/hardt/hardt.h.in"
  "${PROJECT_BINARY_DIR}/src/hardt/hardt.h"
)

add_subdirectory (src)

