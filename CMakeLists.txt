cmake_minimum_required (VERSION 2.6)

project (Hardt)

set (Hardt_VERSION_MAJOR 1)
set (Hardt_VERSION_MINOR 2)
set (Hardt_VERSION_BUILD 3)
set (Hardt_VERSION Hardt_VERSION_MAJOR + "." + HARDT_VERSION_MINOR + "." + HARDT_VERSION_BUILD)

set(CMAKE_CXX_COMPILER "clang++")

configure_file (
  "${PROJECT_SOURCE_DIR}/src/hardt/hardt.h.in"
  "${PROJECT_BINARY_DIR}/src/hardt/hardt.h"
)

add_subdirectory (src)

# Generate documentation from in-source documentation and structure
find_package(Doxygen REQUIRED dot)

if(DOXYGEN_FOUND)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf.in ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf @ONLY)

    ADD_CUSTOM_TARGET(documentation ALL
        COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf
        COMMAND ${CMAKE_COMMAND} -E echo "Done.")

else()

    message("You must install Doxygen and Graphviz to generate the documentation")

endif()
