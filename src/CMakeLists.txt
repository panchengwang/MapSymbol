cmake_minimum_required(VERSION 3.14)

project(mapsymbol LANGUAGES C)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSON_C REQUIRED json-c)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)

list(APPEND HEADERS 
  symbol.h
)

list(APPEND SOURCES 
  symbol.c
  shape.c
  arc.c 
  chord.c 
  pie.c 
  ellipse.c 
  circle.c 
  linestring.c 
  polygon.c 
  regularpolygon.c 
  star.c 
  color.c 
  stroke.c 
  point.c 
  systemline.c 
  systemfill.c 
  fill.c
)

add_library(mapsymbol SHARED
  ${HEADERS}
  ${SOURCES}
)

target_link_libraries(mapsymbol 
  ${JSON_C_LIBRARIES}
  ${CAIRO_LIBRARIES}
  ${GLIB2_LIBRARIES}
)
target_compile_definitions(mapsymbol PRIVATE AS_LIBRARY)
target_include_directories(mapsymbol PUBLIC 
  ${JSON_C_INCLUDE_DIRS}
  ${CAIRO_INCLUDE_DIRS}
  ${GLIB2_INCLUDE_DIRS}
)

target_compile_options(mapsymbol PUBLIC 
  ${JSON_C_CFLAGS_OTHER}
  ${CAIRO_CFLAGS_OTHER}
  ${GLIB2_CFLAGS_OTHER}
  )

include(GNUInstallDirs)

install(TARGETS mapsymbol
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES symbol.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mapsymbol
)