set(CMAKE_MACOSX_RPATH 1)

add_library(hardt SHARED "")
target_link_libraries(hardt m portaudio)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
set_target_properties( hardt PROPERTIES
	VERSION ${Hardt_VERSION_MAJOR}.${Hardt_VERSION_MINOR}
	SOVERSION ${Hardt_VERSION_MAJOR} )

target_sources(hardt
    PRIVATE
        hardt.cpp
        hsoundcard.cpp
        hwav.cpp
        hexceptions.cpp
        hobject.cpp
        htimer.cpp
)

include_directories("${PROJECT_BINARY_DIR}/hardt/libhardt/include")

set_target_properties(hardt PROPERTIES PUBLIC_HEADER include/hardtapi.h)

target_include_directories(hardt PRIVATE . include writers readers readers/generators mixers processors converters outputs filters filters/windows filters/biquads components)

include(GNUInstallDirs)
install(TARGETS hardt
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/hardt)

install(FILES ${CMAKE_BINARY_DIR}/hardt.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/hardt/libhardt/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/hardt)
install(FILES ${CMAKE_BINARY_DIR}/hardt/libhardt/include/hardt.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/hardt)

install(FILES ${CMAKE_SOURCE_DIR}/hardt/template/main.cpp ${CMAKE_SOURCE_DIR}/hardt/template/CMakeLists.txt DESTINATION share/doc/hardt/template)