#ifndef __SYM_SCOLOR_H
#define __SYM_SCOLOR_H

#include <stdint.h>
#include <json.h>
#include <string>
#include "DLLExport.h"


class DLL_EXPORT SColor
{
public:
    SColor();
    SColor(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue);
    virtual ~SColor();

    bool fromJsonObject(json_object* obj, std::string& errorMessage);
    json_object* toJsonObject();

    uint8_t blue() const;
    uint8_t alpha() const;
    uint8_t red() const;
    uint8_t green() const;

    size_t memSize();
    unsigned char* serialize(unsigned char* data);
    unsigned char* deserialize(unsigned char* data);

    std::string toString() ;

protected:
    uint8_t _alpha;
    uint8_t _red;
    uint8_t _green;
    uint8_t _blue;
};

#endif // SCOLOR_H
