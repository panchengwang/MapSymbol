#include <iostream>
#include <SSymbol.h>
#include <string>
#include <sstream>
#include <SBytesCharTransformer.h>
#include <string.h>



void saveToFile(const std::string& filename, unsigned char* buf, size_t len);

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
        SBytesCharTransformer trans;

        size_t hexsize;

        uint8_t *hexbytes;
        hexbytes = trans.bytesToHexBytes(buf,len,hexsize);
        for(size_t i=0; i<hexsize; i++){
            std::cout << hexbytes[i] ;
        }
        std::cout << std::endl;
        bool ok;
        uint8_t* bytes = trans.bytesFromHexBytes(hexbytes, hexsize, ok);
        SSymbol sym3;
        sym3.deserialize((unsigned char*)bytes);
        std::cout << sym3.toJsonString() << std::endl;
        delete [] hexbytes;

        symbol.deserialize(buf);
        delete[] buf;

        for(size_t i=0; i<sym3.nShapes(); i++){
            // unsigned char* buf;
            // size_t len;
            // buf = symbol.shapeToImage(i,"png",72/25.4,len);
            // // std::string myfilename;
            std::stringstream strout;
            strout << filename << i << ".png";
            // saveToFile(strout.str(),buf,len);
            // delete [] buf;
            SSymbol *mysym = sym3.clone(i);
            mysym->toImage(strout.str().c_str());
            delete mysym;
        }
        sym3.toImage(std::string(std::string(filename) + ".png").c_str());


    }


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
