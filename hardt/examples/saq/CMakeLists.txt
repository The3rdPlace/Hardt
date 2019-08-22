cmake_minimum_required(VERSION 3.7)

find_package(Hardt CONFIG)

add_executable (saq saq.cpp)

include_directories(${Hardt_INCLUDE_DIRS})
target_link_libraries (saq ${Hardt_LIBRARIES})

