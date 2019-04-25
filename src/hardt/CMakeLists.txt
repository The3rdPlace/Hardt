set(CMAKE_MACOSX_RPATH 1)

add_library(hardt SHARED "")
target_link_libraries(hardt m portaudio)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

target_sources(hardt
    PRIVATE
        hardt.cpp
        generator.cpp
        hsoundcard.cpp
        hwav.cpp
)

include_directories("${PROJECT_BINARY_DIR}/src/hardt/")

set_target_properties(hardt PROPERTIES PUBLIC_HEADER include/hardtapi.h)

target_include_directories(hardt PRIVATE . include)

include(GNUInstallDirs)
install(TARGETS hardt
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_file(hardt.pc.in hardt.pc @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/hardt.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
