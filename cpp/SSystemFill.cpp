#include "SShape.h"
#include "SSystemFill.h"
#include "JsonHelper.h"
#include <cairo.h>
#include "SCanvas.h"
#include <math.h>
#include "SSolidFill.h"
#include "SerializeHelper.h"



SSystemFill::SSystemFill()
{
    _type = SShape::SYSTEM_FILL;
    _fill = NULL;
}

SSystemFill::~SSystemFill()
{
    clear();
}


void SSystemFill::clear() {
    if (_fill) {
        delete _fill;
    }

    _fill = NULL;
}



bool SSystemFill::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    clear();
    _type = SShape::SYSTEM_FILL;
    json_object* fillobj;
    JSON_GET_OBJ(obj, "fill", fillobj, errorMessage);
    std::string str;
    JSON_GET_STRING(fillobj, "type", str, errorMessage);

    bool ret = false;

    if (str == "solid") {
        _fill = new SSolidFill();
    }

    if (_fill) {
        ret = _fill->fromJsonObject(fillobj, errorMessage);
    }
    else {
        errorMessage = "Unknow fill type: " + str;
        return false;
    }
    return ret;
}

json_object* SSystemFill::toJsonObject()
{
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "systemfill");
    JSON_ADD_OBJ(obj, "fill", _fill->toJsonObject());
    return obj;
}


SRect SSystemFill::getMBR() {
    return SRect(-1, -1, 1, 1);
}

void SSystemFill::draw(SCanvas& canvas) {
    cairo_t* cairo = canvas.cairo();
    // cairo_save(cairo);
    // cairo_restore(cairo);

    canvas.setFill(_fill);
    cairo_paint(cairo);

}


size_t SSystemFill::memSize() {
    size_t len = 0;
    len += sizeof(_type);

    len += _fill->memSize();

    return len;
}

unsigned char* SSystemFill::serialize(unsigned char* data) {
    unsigned char* p = data;
    SERIALIZE(p, _type);
    p = _fill->serialize(p);
    return p;
}

unsigned char* SSystemFill::deserialize(unsigned char* data) {
    clear();

    unsigned char* p = data;
    DESERIALIZE(p, _type);

    uint8_t filltype;
    memcpy(&filltype, p, sizeof(filltype));
    if (filltype == SFill::SOLID) {
        _fill = new SSolidFill();
    }
    p = _fill->deserialize(p);
    return p;
}


SShape* SSystemFill::clone()
{
    SSystemFill *sym = new SSystemFill();
    sym->_type = _type;
    if(_fill){
        sym->_fill = _fill->clone();
    }
    return sym;
}


