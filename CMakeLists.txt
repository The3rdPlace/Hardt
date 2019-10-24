cmake_minimum_required (VERSION 3.7)

# Project name
project (Hardt)

# Set the framework major and minor version here
set (Hardt_VERSION_MAJOR 1)
set (Hardt_VERSION_MINOR 0)

# When setting the debian package version, do not forget to add a changelog
# entry in hardt/debian/changelog
set (Hardt_VERSION_DEB_PACKAGE 4)

# Define the version string
set (Hardt_VERSION Hardt_VERSION_MAJOR + "." + HARDT_VERSION_MINOR + "." + HARDT_VERSION_DEP_PACKAGE)

# Use the CLang compiler
set(CMAKE_CXX_COMPILER "clang++")

# Configure the hardt.h header
configure_file (
  "${PROJECT_SOURCE_DIR}/hardt/libhardt/include/hardt.h.in"
  "${PROJECT_BINARY_DIR}/hardt/libhardt/include/hardt.h"
)

# Configure the hardt.pc file
configure_file (
  "${PROJECT_SOURCE_DIR}/hardt.pc.in"
  "${PROJECT_BINARY_DIR}/hardt.pc"
)

# Configure the HardtConfig.cmake file
configure_file(
  "HardtConfig.cmake.in"
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/HardtConfig.cmake" @ONLY
)

# Install the HardtConfig.cmake
install(
  FILES
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/HardtConfig.cmake"
  DESTINATION "lib/cmake/Hardt" COMPONENT dev
)

# Build projects in this folder
add_subdirectory (hardt)

# Generate documentation from in-source documentation and structure
find_package(Doxygen COMPONENTS dot)
if(DOXYGEN_FOUND)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

    ADD_CUSTOM_TARGET(documentation ALL
	COMMAND sudo rm -r -f ${CMAKE_CURRENT_BINARY_DIR}/docs
        COMMAND ${DOXYGEN_EXECUTABLE}
	COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/docs/examples
	COMMAND sh -c 'ls -d ${CMAKE_CURRENT_SOURCE_DIR}/hardt/examples/*/ | rev | cut -d "/" -f 2 | rev | xargs -I % mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/docs/examples/%'
        COMMAND sh -c 'ls -d ${CMAKE_CURRENT_SOURCE_DIR}/hardt/examples/*/ | rev | cut -d "/" -f 2 | rev | xargs -I % sh -c "cp ${CMAKE_CURRENT_SOURCE_DIR}/hardt/examples/%/* ${CMAKE_CURRENT_BINARY_DIR}/docs/examples/%/." ')

    install(DIRECTORY ${CMAKE_BINARY_DIR}/docs/html/ DESTINATION share/doc/hardt/html)
    install(DIRECTORY ${CMAKE_BINARY_DIR}/docs/examples/ DESTINATION share/doc/hardt/examples)

    # Package into debian package
    if(UNIX AND NOT APPLE)
    	ADD_CUSTOM_TARGET( dist
	    COMMAND sudo rm -r -f dist
	    COMMAND mkdir -p dist/hardt
	    COMMAND cp -r hardt/debian dist/hardt/.
	    COMMAND tar -c -v -f 'dist/hardt_${Hardt_VERSION_MAJOR}.${Hardt_VERSION_MINOR}.orig.tar.gz' -z 'CMakeLists.txt' 'hardt'
	    COMMAND cd dist/hardt && debuild -us -uc --root-command=sudo
	    COMMAND sudo chown -R $ENV{USER} dist
	    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

    	add_dependencies( dist integrationtest documentation )

 	ADD_CUSTOM_TARGET( publish
            COMMAND ../publish.sh)

    endif()

else()

    message("You must install Doxygen and Graphviz to generate the documentation")

endif()
