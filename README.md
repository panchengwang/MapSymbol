# MapSymbol

MapSymbol is a library to provide map symbol definition and drawing for Geographical Infromation System(GIS) applications.


## Installation

```shell
cd ~
https://github.com/panchengwang/MapSymbol.git
cd MapSymbol
mkdir build
cd build 
cmake -DCMAKE_INSTALL_PREFIX=<Path to install mapsymbol> -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
cmake --build . --target install 
```

Sometimes, you must enable sudo previllege before cmake :

```shell
sudo cmake --build . --target install 
```

For example , if the \<Path to install mapsymbol\> is /usr/local, the library files  libmapsymbol.so and libmapsymbol_c.so should located in /usr/local/lib, the header files will be copied to /usr/local/include/mapsymbol and  /usr/local/include/mapsymbol_c/.


## Usage 

Create new folder which your test source file located in :

```shell
cd ~
mkdir TestMapSymbol
cd TestMapSymbol
touch test_mapsymbol.cpp
```

Edit test_mapsymbol.cpp, input sources as following:

```cpp
#include <iostream>
#include <SSymbol.h>
#include <string>
#include <sstream>
#include <SBytesCharTransformer.h>
#include <string.h>



void saveToFile(const std::string& filename, unsigned char* buf, size_t len);
std::string readAllContent(const char* filename);



int main(int argc, char** argv) {

    const char* filename = argv[1];


    std::string jsonstr = readAllContent(filename);
    std::cout << "The source json string : " << std::endl;
    std::cout << jsonstr << std::endl;

    std::cout << "Test map symbol parser......." << std::endl;
    SSymbol sym1;
    if(!sym1.fromJsonString(jsonstr.c_str())){
        std::cout << sym1.errorMessage() << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << " parse json string success ....." << std::endl;
    std::string jsonstr2 = sym1.toJsonString(true);
    std::cout << jsonstr2 << std::endl;
    std::cout << " encode to json string success " << std::endl;

    std::cout << "Test serialize and deserialize ......" << std::endl;
    size_t len=0;
    uint8_t* serializeddata =  sym1.serialize(len);
    std::cout << "Serialize to memory will occupy " << len << " bytes" << std::endl;
    SSymbol sym2;
    sym2.deserialize(serializeddata);
    delete [] serializeddata;
    std::cout << "Deserialize success .... " << std::endl;
    std::cout << "The result is : " << std::endl;
    std::cout << sym2.toJsonString(true) << std::endl;


    std::cout << "Test wkb encoder and decoder ...." << std::endl;
    std::cout << "The WKB(hex string) of the symbol is : " << std::endl;
    std::string hexstr = sym2.toWKB();
    std::cout << hexstr << std::endl;

    SSymbol sym3;
    sym3.fromWKB(hexstr);
    std::cout << " decode from wkb: " << std::endl;
    std::cout << sym3.toJsonString() << std::endl;


    std::cout << "Test symbol to image: " << std::endl;
    uint8_t* img = sym3.toImage("jpg",144/25.4,len);
    saveToFile(std::string(filename) + ".jpg" , img,len);
    return EXIT_SUCCESS;
}


void saveToFile(const std::string& filename, unsigned char* buf, size_t len){

    FILE* fd = fopen(filename.c_str(), "wb");
    if (!fd) {
        std::cout << "Can not write to image file: " << filename << std::endl;
        return ;
    }
    fwrite(buf, 1, len, fd);
    fclose(fd);
}



std::string readAllContent(const char* filename)
{
    FILE* fd = fopen(filename, "rb");
    if (!fd) {
        return std::string();
    }
    fseek(fd, 0, SEEK_END);
    std::size_t len = ftell(fd);

    char* buf = new char[len + 1];
    buf[len] = '\0';

    fseek(fd, 0, SEEK_SET);
    fread(buf, 1, len, fd);
    fclose(fd);
    std::string data = buf;
    delete[] buf;
    return data;
}

```

Create CMakeLists.txt:

```cmake
cmake_minimum_required(VERSION 3.5)

project(test_mapsymbol LANGUAGES CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(JSON_C REQUIRED json-c)

add_executable(test_mapsymbol test_mapsymbol.cpp )
target_include_directories(test_mapsymbol PRIVATE
    ${CAIRO_INCLUDE_DIRS}
    ${JSON_C_INCLUDE_DIRS}
    /usr/local/include/mapsymbol
)
target_link_directories(test_mapsymbol PRIVATE
    ${CAIRO_LIB_DIRS}
    ${JSON_C_LIB_DIRS}
    /usr/local/lib
)
target_link_libraries(test_mapsymbol
    ${CAIRO_LIBRARIES}
    ${JSON_C_LIBRARIES}
    mapsymbol
)


include(GNUInstallDirs)


```
Create a map symbol json file star.json, the content is :

```json
{
    "offset": {
        "x": 0,
        "y": 0
    },
    "xscale": 50.0,
    "yscale": 50.0,
    "size": 50,
    "shapes": [
        {
            "type": "path",
            "stroke": {
                "color": {
                    "alpha": 255,
                    "red": 255,
                    "green": 0,
                    "blue": 0
                },
                "width": 0.0001,
                "dashoffset": 0,
                "dashes": [
                    1.0,
                    0
                ],
                "cap": "butt",
                "join": "miter",
                "miter": 10.0
            },
            "fill": {
                "type": "solid",
                "color": {
                    "alpha": 255,
                    "red": 255,
                    "green": 0,
                    "blue": 0
                }
            },
            "subpaths": [
                {
                    "type": "star",
                    "center": {
                        "x": -0,
                        "y": -0
                    },
                    "outerradius": 0.79,
                    "innerradius": 0.3,
                    "rotate": 0,
                    "numedges": 5
                }
            ],
            "closed": true
        }
    ]
}
```

Compile test source code:

```shell
mkdir build
cd build
cmake ..
cmake --build .

```

Run :

```shell
./test_symbol ../star.json
```