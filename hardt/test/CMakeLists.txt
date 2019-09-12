# add the executable
add_executable (hardt_test test.cpp)
target_link_libraries (hardt_test hardt pthread)

target_sources(hardt_test
    PRIVATE
	hfilewriter_test.cpp
	hnetwork_test.cpp
	hwavwriter_test.cpp
	hfilereader_test.cpp
	hwavreader_test.cpp
	hgenerator_test.cpp
	hsinegenerator_test.cpp
	hcosinegenerator_test.cpp
	hvfo_test.cpp
	hstreamprocessor_test.cpp
	hfilter_test.cpp
	hconverter_test.cpp
	houtput_test.cpp
	hnullwriter_test.cpp
	hchunkwriter_test.cpp
	hwindow_test.cpp
	hfft_test.cpp
	hrectangularwindow_test.cpp
	hhammingwindow_test.cpp
	hhahnwindow_test.cpp
	hlinearmixer_test.cpp
	hfirfilter_test.cpp
	hiirfilter_test.cpp
	hbiquads_test.cpp
	hgain_test.cpp
	hcombfilter_test.cpp
	hgoertzelfilter_test.cpp
	hsplitter_test.cpp
	hsignallevel_test.cpp
	hdemux_test.cpp
	hmux_test.cpp
	hwriterconsumer_test.cpp
	hswitch_test.cpp
	hmute_test.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

include_directories("${PROJECT_SOURCE_DIR}/hardt/test"
	"${PROJECT_SOURCE_DIR}/hardt/libhardt" 
	"${PROJECT_SOURCE_DIR}/hardt/libhardt/include"
	"${PROJECT_BINARY_DIR}/hardt/libhardt/include")

target_include_directories(hardt_test PRIVATE . include)

ADD_CUSTOM_TARGET(unittests ALL COMMAND hardt_test ${CMAKE_CURRENT_SOURCE_DIR} > test.log || cat test.log)
ADD_CUSTOM_TARGET(integrationtest COMMAND hardt_test ${CMAKE_CURRENT_SOURCE_DIR} -it)
