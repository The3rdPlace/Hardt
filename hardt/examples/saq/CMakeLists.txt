cmake_minimum_required(VERSION 3.7)

#if (NOT Hardt_VERSION)
  find_package(Hardt CONFIG)
#endif()

# Include directories
include_directories(${Hardt_INCLUDE_DIRS})

# Add the executable
add_executable (saq saq.cpp)

# Libraries
target_link_libraries (saq ${Hardt_LIBRARIES})
