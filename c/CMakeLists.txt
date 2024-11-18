cmake_minimum_required(VERSION 3.14)

project(mapsymbol_c LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_compile_options( -Wextra -Wpedantic -Wno-unused-result)


find_package(PkgConfig REQUIRED)
pkg_check_modules(JSON_C REQUIRED json-c)
pkg_check_modules(CAIRO REQUIRED cairo)


list(APPEND HEADERS 
	symbol.h
)

list(APPEND SOURCES 
	symbol.cpp
)

add_library(mapsymbol_c SHARED
  ${HEADERS}
  ${SOURCES}
)

target_link_directories(mapsymbol_c PRIVATE
    ../cpp
)

target_link_libraries(mapsymbol_c 
	mapsymbol
)
target_compile_definitions(mapsymbol_c PRIVATE AS_LIBRARY)
target_include_directories(mapsymbol_c PUBLIC 
	../cpp
)



include(GNUInstallDirs)

install(TARGETS mapsymbol_c
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(TARGET mapsymbol_c POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mapsymbol_c> ${CMAKE_BINARY_DIR}/bin/$<TARGET_FILE_NAME:mapsymbol_c>
)

install(FILES ${HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mapsymbol_c
)
