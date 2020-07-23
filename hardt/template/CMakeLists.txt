cmake_minimum_required(VERSION 3.7)
set( APPNAME MyHardtApp )

find_package(Hardt CONFIG)

add_definitions(-DAPPNAME="${APPNAME}")
include_directories(${Hardt_INCLUDE_DIRS})

add_executable (${APPNAME} main.cpp)

target_link_libraries (${APPNAME} ${Hardt_LIBRARIES})

