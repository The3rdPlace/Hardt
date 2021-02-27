# add the executable
add_executable (hardt_test test.cpp)
target_link_libraries (hardt_test hardt pthread)

target_sources(hardt_test
    PRIVATE
    hagc_test.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

include_directories("${PROJECT_SOURCE_DIR}/hardt/test"
	"${PROJECT_SOURCE_DIR}/hardt/libhardt" 
	"${PROJECT_SOURCE_DIR}/hardt/libhardt/include"
	"${PROJECT_BINARY_DIR}/hardt/libhardt/include")

target_include_directories(hardt_test PRIVATE . include)

ADD_CUSTOM_TARGET(tests ALL COMMAND hardt_test ${CMAKE_CURRENT_SOURCE_DIR} > test.log || cat test.log)
ADD_CUSTOM_TARGET(unittests COMMAND hardt_test ${CMAKE_CURRENT_SOURCE_DIR} -ut > test.log || cat test.log)
ADD_CUSTOM_TARGET(integrationtests COMMAND hardt_test ${CMAKE_CURRENT_SOURCE_DIR} -it)

add_executable (template_test ../template/main.cpp)
target_link_libraries (template_test hardt)
add_definitions(-DAPPNAME="template_test")
