# add the executable
add_executable (hardt_test test.cpp)
target_link_libraries (hardt_test hardt pthread)

target_sources(hardt_test
    PRIVATE
        hfilewriter_test.cpp
	    #hnetwork_test.cpp
	    hwavwriter_test.cpp
	    hfilereader_test.cpp
	    hwavreader_test.cpp
	    #hgenerator_test.cpp
	    #hsinegenerator_test.cpp
	    #hcosinegenerator_test.cpp
	    #hvfo_test.cpp
	    hstreamprocessor_test.cpp
	    hfilter_test.cpp
	    hconverter_test.cpp
	    houtput_test.cpp
	    hnullwriter_test.cpp
	    hchunkwriter_test.cpp
	    hwindow_test.cpp
	    #hfft_test.cpp
	    hrectangularwindow_test.cpp
	    hhammingwindow_test.cpp
	    hhahnwindow_test.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

include_directories("${PROJECT_SOURCE_DIR}/src/test"
	"${PROJECT_SOURCE_DIR}/src/hardt" 
	"${PROJECT_SOURCE_DIR}/src/hardt/include"
	"${PROJECT_BINARY_DIR}/src/hardt")

target_include_directories(hardt_test PRIVATE . include)

ADD_CUSTOM_TARGET(run_unittests ALL COMMAND hardt_test ${CMAKE_CURRENT_SOURCE_DIR})
