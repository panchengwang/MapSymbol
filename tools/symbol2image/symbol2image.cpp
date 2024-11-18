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
