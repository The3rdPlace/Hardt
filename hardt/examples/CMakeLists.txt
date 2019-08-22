string(APPEND CMAKE_CXX_FLAGS " --std=c++11 ")
set(Hardt_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/hardt/libhardt/include ${PROJECT_BINARY_DIR}/hardt/libhardt/include)
set(Hardt_LIBRARIES hardt)

add_subdirectory (saq)
