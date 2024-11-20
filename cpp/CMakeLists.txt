cmake_minimum_required(VERSION 3.14)

project(mapsymbol LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_compile_options( -Wextra -Wpedantic -Wno-unused-result)


find_package(PkgConfig REQUIRED)
pkg_check_modules(JSON_C REQUIRED json-c)
pkg_check_modules(CAIRO REQUIRED cairo)


list(APPEND HEADERS 
  DLLExport.h
  JsonHelper.h
  SerializeHelper.h
  SSymbol.h
  SColor.h
  SShape.h
  SPoint.h 
  SRect.h 
  SFill.h
  SSolidFill.h
  SStroke.h
  SSystemLine.h
  SSystemFill.h
  SSubPath.h
  SCircle.h
  SArc3Points.h
  SRegularPolygon.h
  SArc.h
  SPie.h
  SEllipse.h
  SChord.h
  SStar.h
  SPath.h
  SCanvas.h
  SLineString.h
  SPolygon.h
  SText.h
  SBytesCharTransformer.h
)

list(APPEND SOURCES 
  SerializeHelper.cpp
  SSymbol.cpp
  SColor.cpp
  SShape.cpp
  SPoint.cpp 
  SRect.cpp 
  SFill.cpp
  SSolidFill.cpp
  SStroke.cpp
  SSystemLine.cpp
  SSystemFill.cpp
  SSubPath.cpp
  SCircle.cpp
  SArc3Points.cpp
  SRegularPolygon.cpp
  SArc.cpp
  SPie.cpp
  SEllipse.cpp
  SChord.cpp
  SStar.cpp
  SPath.cpp
  SCanvas.cpp
  SLineString.cpp
  SPolygon.cpp
  SText.cpp
  SBytesCharTransformer.cpp
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

add_custom_command(TARGET mapsymbol POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mapsymbol> ${CMAKE_BINARY_DIR}/bin/$<TARGET_FILE_NAME:mapsymbol>
)

install(FILES ${HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mapsymbol
)
