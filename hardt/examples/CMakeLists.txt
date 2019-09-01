set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
set( CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR})
set( Hardt_DIR ${PROJECT_SOURCE_DIR})

include_directories("${PROJECT_BINARY_DIR}/hardt/libhardt/include")
include_directories("${PROJECT_SOURCE_DIR}/hardt/libhardt/include")

add_subdirectory (saq)
add_subdirectory (simple)
add_subdirectory (readers_and_writers)
add_subdirectory (readers_and_consumers)
add_subdirectory (network)