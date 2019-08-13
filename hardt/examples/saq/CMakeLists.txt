# Locate headers when compiling as part of Hardt
# These 3 lines is only needed for example builds - as part of building Hardt locally
include_directories("${PROJECT_SOURCE_DIR}/hardt/libhardt/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/../../../hardt/libhardt")

# When using a binary distribution of Hardt, include files with these lines
# Todo: Add include_directories() for bin. dist.

# When using a binary distribution of Hardt, locate Hardt
# Todo: Add find_package()'ish lines here to let the compiler find the library

# Add the executable
add_executable (saq saq.cpp)
target_link_libraries (saq hardt)

# Extra flags, we would like - as a minimum - C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
