cmake_minimum_required (VERSION 2.6)

project (Hardt)

# Set the framework major and minor version here
set (Hardt_VERSION_MAJOR 1)
set (Hardt_VERSION_MINOR 2)

# When setting the debian package version, do not forget to add a changelog
# entry in hardt/debian/changelog
set (Hardt_VERSION_DEB_PACKAGE 3)


set (Hardt_VERSION Hardt_VERSION_MAJOR + "." + HARDT_VERSION_MINOR + "." + HARDT_VERSION_DEP_PACKAGE)

set(CMAKE_CXX_COMPILER "clang++")

configure_file (
  "${PROJECT_SOURCE_DIR}/hardt/libhardt/include/hardt.h.in"
  "${PROJECT_BINARY_DIR}/hardt/libhardt/include/hardt.h"
)

configure_file (
  "${PROJECT_SOURCE_DIR}/hardt.pc.in"
  "${PROJECT_BINARY_DIR}/hardt.pc"
)

add_subdirectory (hardt)

# Generate documentation from in-source documentation and structure
find_package(Doxygen REQUIRED dot)

if(DOXYGEN_FOUND)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

    ADD_CUSTOM_TARGET(documentation ALL
        COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
        #COMMAND ${DOXYGEN_EXECUTABLE}
        COMMAND ${CMAKE_COMMAND} -E echo "Done."
	COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/docs/examples
	COMMAND sh -c 'ls -d ${CMAKE_CURRENT_SOURCE_DIR}/hardt/examples/*/ | rev | cut -d "/" -f 2 | rev | xargs -I % mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/docs/examples/%'
        COMMAND sh -c 'ls -d ${CMAKE_CURRENT_SOURCE_DIR}/hardt/examples/*/ | rev | cut -d "/" -f 2 | rev | xargs -I % sh -c "cp ${CMAKE_CURRENT_SOURCE_DIR}/hardt/examples/%/* ${CMAKE_CURRENT_SOURCE_DIR}/docs/examples/%/." ')

    install(DIRECTORY ${CMAKE_SOURCE_DIR}/docs/html/ DESTINATION share/doc/hardt)

else()

    message("You must install Doxygen and Graphviz to generate the documentation")

endif()

# Package into debian package
ADD_CUSTOM_TARGET( dist
	COMMAND sudo rm -r -f dist
	COMMAND mkdir -p dist/hardt
	COMMAND cp -r hardt/debian dist/hardt/.
	COMMAND tar -c -v -f 'dist/hardt_${Hardt_VERSION_MAJOR}.${Hardt_VERSION_MINOR}.orig.tar.gz' -z 'CMakeLists.txt' 'hardt' 'docs'
	COMMAND cd dist/hardt && debuild -us -uc --root-command=sudo
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})


