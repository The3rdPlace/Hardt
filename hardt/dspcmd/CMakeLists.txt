include_directories("${PROJECT_BINARY_DIR}/hardt/libhardt/include")
include_directories("${PROJECT_SOURCE_DIR}/hardt/libhardt/include")
include_directories ("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/hardt/dspcmd/include")

# add the executable
add_executable (dspcmd dspcmd.cpp config.cpp operations.cpp signalhandling.cpp)
target_link_libraries (dspcmd hardt pthread)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

include(GNUInstallDirs)
install(FILES ${CMAKE_BINARY_DIR}/hardt/dspcmd/dspcmd DESTINATION ${CMAKE_INSTALL_BINDIR})
