#ifndef __SYM_SBYTESCHARTRANSFORMER_H
#define __SYM_SBYTESCHARTRANSFORMER_H

#include <string>
#include <stdint.h>
#include "DLLExport.h"



 class DLL_EXPORT SBytesCharTransformer
{
public:
    // static uint8_t HEX2CHAR[256] ;
public:
    SBytesCharTransformer();

    uint8_t* bytesFromHexBytes(uint8_t *hexbuf, size_t hexsize, bool& ok);
    uint8_t* bytesToHexBytes(uint8_t* bytes, size_t byteSize, size_t& hexSize);

    const std::string& errorMessage();

    std::string bytesToHexString(uint8_t* bytes, size_t byteSize);
 protected:
    std::string _errorMessage;
};

// uint8_t SBytesCharTransformer::

#endif // SBYTESCHARTRANSFORMER_H
