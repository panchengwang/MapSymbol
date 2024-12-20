#include "SColor.h"
#include "JsonHelper.h"
#include "SerializeHelper.h"
#include <sstream>


SColor::SColor()
{
    _alpha = 255;
    _red = 255;
    _green = 255;
    _blue = 255;
}

SColor::SColor(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue)
{
    _alpha = alpha;
    _red = red;
    _green = green;
    _blue = blue;
}

SColor::~SColor()
{

}

bool SColor::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    JSON_GET_INT(obj, "alpha", _alpha, errorMessage);
    JSON_GET_INT(obj, "red", _red, errorMessage);
    JSON_GET_INT(obj, "green", _green, errorMessage);
    JSON_GET_INT(obj, "blue", _blue, errorMessage);
    return true;
}

json_object* SColor::toJsonObject()
{
    json_object* obj = json_object_new_object();

    JSON_ADD_INT(obj, "alpha", _alpha);
    JSON_ADD_INT(obj, "red", _red);
    JSON_ADD_INT(obj, "green", _green);
    JSON_ADD_INT(obj, "blue", _blue);

    return obj;
}

uint8_t SColor::blue() const
{
    return _blue;
}

uint8_t SColor::alpha() const
{
    return _alpha;
}

uint8_t SColor::red() const
{
    return _red;
}

uint8_t SColor::green() const
{
    return _green;
}



size_t SColor::memSize() {
    return sizeof(_alpha) + sizeof(_red) + sizeof(_green) + sizeof(_blue);
}


unsigned char* SColor::serialize(unsigned char* data) {
    unsigned char* p = data;
    SERIALIZE(p, _alpha);
    SERIALIZE(p, _red);
    SERIALIZE(p, _green);
    SERIALIZE(p, _blue);
    return p;
}

unsigned char* SColor::deserialize(unsigned char* data) {
    unsigned char* p = data;
    DESERIALIZE(p, _alpha);
    DESERIALIZE(p, _red);
    DESERIALIZE(p, _green);
    DESERIALIZE(p, _blue);
    return p;
}

std::string SColor::toString()
{
    std::stringstream outstr;
    // outstr << "Color(red: " << (int)_red << ", green: " << (int)_green << ", blue: " << (int)_blue << ", alpha: " << (int)_alpha << ")" ;
    return outstr.str();
}

