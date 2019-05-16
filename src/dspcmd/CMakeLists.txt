include_directories("${PROJECT_BINARY_DIR}/src/hardt")
include_directories("${PROJECT_SOURCE_DIR}/src/hardt/include")
include_directories ("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/src/dspcmd/include")

# add the executable
add_executable (dspcmd dspcmd.cpp config.cpp operations.cpp signalhandling.cpp)
target_link_libraries (dspcmd hardt pthread)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
