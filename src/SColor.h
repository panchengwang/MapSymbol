#ifndef SCOLOR_H
#define SCOLOR_H

#include <stdint.h>
#include <json.h>
#include <string>



class SColor
{
public:
    SColor();
    SColor(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue);
    virtual ~SColor();

    virtual bool fromJsonObject(json_object* obj, std::string& errorMessage)  ;
    virtual json_object* toJsonObject()  ;

    uint8_t blue() const;
    uint8_t alpha() const;
    uint8_t red() const;
    uint8_t green() const;

protected:
    uint8_t _alpha;
    uint8_t _red;
    uint8_t _green;
    uint8_t _blue;
};

#endif // SCOLOR_H
