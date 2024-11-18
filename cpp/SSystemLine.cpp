#include "SShape.h"
#include "SSystemLine.h"
#include "JsonHelper.h"
#include <cairo.h>
#include "SCanvas.h"
#include <math.h>
#include "SerializeHelper.h"


SSystemLine::SSystemLine()
{
    _type = SShape::SYSTEM_LINE;
    _stroke = new SStroke();
}

SSystemLine::~SSystemLine()
{
    clear();
}


void SSystemLine::clear() {
    if (_stroke) {
        delete _stroke;
    }
    _stroke = NULL;
}



bool SSystemLine::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    clear();

    _type = SShape::SYSTEM_LINE;
    json_object* strokeobj;
    JSON_GET_OBJ(obj, "stroke", strokeobj, errorMessage);
    _stroke = new SStroke();
    if (!_stroke->fromJsonObject(strokeobj, errorMessage)) {
        return false;
    }
    return true;
}

json_object* SSystemLine::toJsonObject()
{
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "systemline");
    JSON_ADD_OBJ(obj, "stroke", _stroke->toJsonObject());
    return obj;
}


SRect SSystemLine::getMBR() {
    double w = _stroke->width();
    return SRect(-1, 0, 1, 0);
}



void SSystemLine::draw(SCanvas& canvas) {
    cairo_t* cairo = canvas.cairo();
    cairo_save(cairo);

    cairo_move_to(cairo, -1, 0);
    cairo_line_to(cairo, 1, 0);

    cairo_restore(cairo);

    canvas.setStroke(_stroke);
    cairo_stroke(cairo);

}



double SSystemLine::getMaxStrokeWidth() {
    return _stroke->width();
}


size_t SSystemLine::memSize() {
    size_t len = 0;
    len += sizeof(_type);

    len += _stroke->memSize();

    return len;
}


unsigned char* SSystemLine::serialize(unsigned char* data) {
    unsigned char* p = data;
    SERIALIZE(p, _type);
    p = _stroke->serialize(p);
    return p;
}

unsigned char* SSystemLine::deserialize(unsigned char* data) {
    unsigned char* p = data;
    DESERIALIZE(p, _type);
    delete _stroke;
    _stroke = new SStroke();
    p = _stroke->deserialize(p);
    return p;
}

SShape* SSystemLine::clone()
{
    SSystemLine *sym = new SSystemLine();
    sym->_type = _type;
    if(_stroke){
        sym->_stroke = _stroke->clone();
    }
    return sym;
}
