# add the executable
add_executable (hardt_test test.cpp)
target_link_libraries (hardt_test hardt)

include_directories("${PROJECT_BINARY_DIR}/src/hardt")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -v")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")

ADD_CUSTOM_TARGET(run_unittests ALL COMMAND hardt_test)
