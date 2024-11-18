cmake_minimum_required(VERSION 3.14)
project(mapsymbol)
add_subdirectory(cpp)
add_subdirectory(c)
add_subdirectory(tools)

# mkdir build
# cd build
# cmake -DCMAKE_INSTALL_PREFIX=`pg_config --bindir`/.. -DCMAKE_BUILD_TYPE=Release ..
# cmake --build .
# cmake --build . --target install 
