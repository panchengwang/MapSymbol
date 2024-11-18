cmake_minimum_required(VERSION 3.5)

project(symbol2image LANGUAGES C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(JSON_C REQUIRED json-c)

add_executable(symbol2image symbol2image.cpp )
target_include_directories(symbol2image PRIVATE
    ${JSON_C_INCLUDE_DIRS}
    ${CAIRO_INCLUDE_DIRS}
    ../../cpp
)
target_link_directories(symbol2image PRIVATE
    ../../cpp
    ${JSON_C_LIB_DIRS}
    ${CAIRO_LIB_DIRS}
)
target_link_libraries(symbol2image
    ${JSON_C_LIBRARIES}
    ${CAIRO_LIBRARIES}
     mapsymbol
)
target_compile_options(symbol2image PUBLIC
  ${CAIRO_CFLAGS_OTHER}
  ${JSON_C_CFLAGS_OTHER}
  )
install(TARGETS symbol2image
    DESTINATION ${PROJECT_SOURCE_DIR}
)

include(GNUInstallDirs)

add_custom_command(TARGET symbol2image POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:symbol2image> ${CMAKE_BINARY_DIR}/bin/$<TARGET_FILE_NAME:symbol2image>
)



