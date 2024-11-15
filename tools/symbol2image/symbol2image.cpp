#include <iostream>
#include <SSymbol.h>
#include <string>





int main(int argc, char** argv) {

    const char* filename = argv[1];

    SSymbol symbol;
    if (!symbol.fromJsonFile(filename)) {
        std::cout << symbol.errorMessage() << std::endl;
    }
    else {
        // std::string jsonstr = symbol.toJsonString(true);
        // if (!symbol.fromJsonString(jsonstr.c_str())) {
        //     std::cout << symbol.errorMessage() << std::endl;
        // }
        // else {
        //     std::cout << symbol.toJsonString(true) << std::endl;
        //     symbol.toImage(std::string(std::string(filename) + ".png").c_str());
        // }
        size_t len=0;
        unsigned char* buf = symbol.serialize(len);
        symbol.deserialize(buf);
        symbol.toImage(std::string(std::string(filename) + ".png").c_str());
        delete[] buf;
    }


    return EXIT_SUCCESS;
}
