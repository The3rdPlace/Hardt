include_directories("hardt")

# Add the executable
add_executable (saq saq.cpp)
target_link_libraries (saq hardt)

# Extra flags, we would like - as a minimum - C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
