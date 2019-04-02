#include_directories ("${PROJECT_SOURCE_DIR}/")
#add_subdirectory (MathFunctions)

# add the executable
add_executable (dspcmd dspcmd.cpp)
target_link_libraries (dspcmd hardt portaudio pthread)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
