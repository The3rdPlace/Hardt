# Local config file for the Hardt package during build
# It defines the following variables
#  Hardt_INCLUDE_DIRS - include directories for Hardt
#  Hardt_LIBRARIES - libraries to link against

set(Hardt_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/hardt/include/hardt  ${PROJECT_BINARY_DIR}/hardt/include/hardt)

set(Hardt_LIBRARIES hardt)

# Extra flags, we would like - as a minimum - C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 ")
