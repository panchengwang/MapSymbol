#include "SBytesCharTransformer.h"
#include <iostream>
#include <sstream>


const uint8_t HEX2CHAR[256] =  {
    /* not Hex characters */
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    /* 0-9 */
    0,1,2,3,4,5,6,7,8,9,20,20,20,20,20,20,
    /* A-F */
    20,10,11,12,13,14,15,20,20,20,20,20,20,20,20,20,
    /* not Hex characters */
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    /* a-f */
    20,10,11,12,13,14,15,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    /* not Hex characters (upper 128 characters) */
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20
};


SBytesCharTransformer::SBytesCharTransformer()
{

}

uint8_t* SBytesCharTransformer::bytesFromHexBytes(uint8_t* hexbuf, size_t hexsize, bool& ok)
{
    uint8_t *buf = NULL;
    uint8_t h1, h2;
    uint32_t i;

    if( hexsize % 2 ){
        _errorMessage = "Invalid hex string, length (%d) has to be a multiple of two!";
        ok = false;
        return NULL;
    }

    buf = new uint8_t[hexsize/2];

    if( ! buf ){
        _errorMessage = "Unable to allocate memory buffer.";
        ok = false;
        return NULL;
    }

    for( i = 0; i < hexsize/2; i++ )
    {
        h1 = HEX2CHAR[(int)hexbuf[2*i]];
        h2 = HEX2CHAR[(int)hexbuf[2*i+1]];
        if( h1 > 15 ){
            _errorMessage = std::string("Invalid hex character (%c) encountered") + (char)hexbuf[2*i];
            ok = false;
            delete [] buf;
            return NULL;
        }
        if( h2 > 15 ){
            _errorMessage = std::string("Invalid hex character (%c) encountered") + (char) hexbuf[2*i+1];
            ok = false;
            delete [] buf;
            return NULL;
        }

        buf[i] = ((h1 & 0x0F) << 4) | (h2 & 0x0F);
    }

    ok = true;
    return buf;
}


std::string SBytesCharTransformer::bytesToHexString(uint8_t* bytes, size_t byteSize){
    std::ostringstream out;
    uint8_t b;
    for(size_t i=0; i<byteSize; i++){
        b = bytes[i];
        out << std::hex << (b>>4)  << (b & 0x0f);
    }
    return out.str();
}

uint8_t* SBytesCharTransformer::bytesToHexBytes(uint8_t* bytes, size_t byteSize, size_t& hexSize)
{
    std::string hexstr = bytesToHexString(bytes,byteSize);
    hexSize = hexstr.size();

    uint8_t* hexbytes = new uint8_t[hexSize];
    memcpy(hexbytes, hexstr.c_str(), hexSize);
    return hexbytes;
}

const std::string& SBytesCharTransformer::errorMessage()
{
    return _errorMessage;
}
