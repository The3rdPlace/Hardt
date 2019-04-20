# add the executable
add_executable (hardt_test test.cpp)
target_link_libraries (hardt_test hardt)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

include_directories("${PROJECT_BINARY_DIR}/src/hardt")

ADD_CUSTOM_TARGET(run_unittests ALL COMMAND hardt_test)
