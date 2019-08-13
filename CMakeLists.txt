cmake_minimum_required (VERSION 2.6)

project (Hardt)

set (Hardt_VERSION_MAJOR 1)
set (Hardt_VERSION_MINOR 2)
set (Hardt_VERSION_BUILD 3)
set (Hardt_VERSION Hardt_VERSION_MAJOR + "." + HARDT_VERSION_MINOR + "." + HARDT_VERSION_BUILD)

set(CMAKE_CXX_COMPILER "clang++")

configure_file (
  "${PROJECT_SOURCE_DIR}/hardt/libhardt/hardt.h.in"
  "${PROJECT_BINARY_DIR}/hardt/libhardt/hardt.h"
)

add_subdirectory (hardt)

# Generate documentation from in-source documentation and structure
find_package(Doxygen REQUIRED dot)

if(DOXYGEN_FOUND)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

    ADD_CUSTOM_TARGET(documentation ALL
        COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
        COMMAND ${DOXYGEN_EXECUTABLE}
        COMMAND ${CMAKE_COMMAND} -E echo "Done.")

else()

    message("You must install Doxygen and Graphviz to generate the documentation")

endif()

# Package into debian package
ADD_CUSTOM_TARGET( dist
	COMMAND ${CMAKE_COMMAND} -E echo_append "Building Debian package"
	COMMAND tar --exclude='.././build' --exclude='.././.git' -c -v -f 'hardt_${Hardt_VERSION_MAJOR}.${Hardt_VERSION_MINOR}.orig.tar.gz' -z '../.')

