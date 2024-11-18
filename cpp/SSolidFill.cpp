#include "SSolidFill.h"
#include "JsonHelper.h"
#include <cairo.h>
#include "SCanvas.h"
#include "SerializeHelper.h"




SSolidFill::SSolidFill()
{
    _type = SFill::SOLID;
}

SSolidFill::~SSolidFill()
{

}

bool SSolidFill::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    _type = SFill::SOLID;
    JSON_GET_COLOR(obj, "color", _color, errorMessage);
    return true;
}

json_object* SSolidFill::toJsonObject()
{
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "solid");
    JSON_ADD_COLOR(obj, "color", _color);
    return obj;
}



void SSolidFill::setTo(SCanvas& canvas) {
    cairo_t* cairo = canvas.cairo();
    cairo_set_source_rgba(cairo,
        _color.red() / 255.0,
        _color.green() / 255.0,
        _color.blue() / 255.0,
        _color.alpha() / 255.0
    );
}


size_t SSolidFill::memSize() {
    size_t len = 0;
    len += sizeof(_type);
    len += _color.memSize();
    return len;
}


unsigned char* SSolidFill::serialize(unsigned char* data) {
    unsigned char* p = data;
    SERIALIZE(p, _type);
    p = _color.serialize(p);
    return p;
}

unsigned char* SSolidFill::deserialize(unsigned char* data) {
    unsigned char* p = data;
    DESERIALIZE(p, _type);
    p = _color.deserialize(p);
    return p;
}

SFill* SSolidFill::clone()
{
    SSolidFill *fill= new SSolidFill();
    fill->_type = _type;
    fill->_color = _color;
    return fill;
}
