# add the executable
add_executable (hardt_test test.cpp)
target_link_libraries (hardt_test hardt pthread)

target_sources(hardt_test
    PRIVATE
        hfilewriter_test.cpp
	    hnetwork_test.cpp
#	    hwavwriter_test.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

include_directories("${PROJECT_SOURCE_DIR}/src/test"
	"${PROJECT_SOURCE_DIR}/src/hardt" 
	"${PROJECT_SOURCE_DIR}/src/hardt/include"
	"${PROJECT_BINARY_DIR}/src/hardt")

target_include_directories(hardt_test PRIVATE . include)

ADD_CUSTOM_TARGET(run_unittests ALL COMMAND hardt_test)
