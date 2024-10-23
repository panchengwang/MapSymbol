cmake_minimum_required(VERSION 3.5)

project(test_symbol LANGUAGES C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)

add_executable(test_symbol test.c)
target_include_directories(test_symbol PRIVATE
    ${GLIB2_INCLUDE_DIRS}
    ../src
)
target_link_directories(test_symbol PRIVATE
    ../src
    ${GLIB2_LIB_DIRS}
)
target_link_libraries(test_symbol
    ${GLIB2_LIBRARIES}
     mapsymbol
)
install(TARGETS test_symbol
    DESTINATION ${PROJECT_SOURCE_DIR}
)

include(GNUInstallDirs)




