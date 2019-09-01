cmake_minimum_required(VERSION 3.7)

find_package(Hardt CONFIG)

add_executable (readers_and_consumers readers_and_consumers.cpp)

include_directories(${Hardt_INCLUDE_DIRS})
target_link_libraries (readers_and_consumers ${Hardt_LIBRARIES})

