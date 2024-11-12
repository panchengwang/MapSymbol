cmake_minimum_required(VERSION 3.14)
project(mapsymbol)
add_subdirectory(src)
add_subdirectory(test)

# mkdir build
# cmake -DCMAKE_INSTALL_PREFIX=`pg_config --bindir`/.. -DCMAKE_BUILD_TYPE=Release ..
# cmake --build .
# cmake --build . --target install 
