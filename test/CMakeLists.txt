cmake_minimum_required(VERSION 3.5)

project(test_symbol LANGUAGES C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
pkg_check_modules(CAIRO REQUIRED cairo)

add_executable(test_symbol test.c)
target_include_directories(test_symbol PRIVATE
    ${GLIB2_INCLUDE_DIRS}
    ${CAIRO_INCLUDE_DIRS}
    ../src
)
target_link_directories(test_symbol PRIVATE
    ../src
    ${GLIB2_LIB_DIRS}
    ${CAIRO_LIB_DIRS}
)
target_link_libraries(test_symbol
    ${GLIB2_LIBRARIES}
    ${CAIRO_LIBRARIES}
     mapsymbol
)
target_compile_options(mapsymbol PUBLIC 
  ${CAIRO_CFLAGS_OTHER}
  ${GLIB2_CFLAGS_OTHER}
  )
install(TARGETS test_symbol
    DESTINATION ${PROJECT_SOURCE_DIR}
)

include(GNUInstallDirs)




