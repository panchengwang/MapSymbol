#include "SEllipse.h"
#include "JsonHelper.h"
#include <math.h>
#include "SCanvas.h"
#include <cairo.h>
#include "SerializeHelper.h"


SEllipse::SEllipse()
{
    _type = SSubPath::ELLIPSE;
}

SEllipse::~SEllipse()
{

}

bool SEllipse::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    _type = SSubPath::ELLIPSE;
    JSON_GET_POINT(obj, "center", _center, errorMessage);
    JSON_GET_DOUBLE(obj, "xradius", _xradius, errorMessage);
    JSON_GET_DOUBLE(obj, "yradius", _yradius, errorMessage);
    JSON_GET_DOUBLE(obj, "rotate", _rotate, errorMessage);
    return true;
}

json_object* SEllipse::toJsonObject()
{
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "ellipse");
    JSON_ADD_POINT(obj, "center", _center);
    JSON_ADD_DOUBLE(obj, "xradius", _xradius);
    JSON_ADD_DOUBLE(obj, "yradius", _yradius);
    JSON_ADD_DOUBLE(obj, "rotate", _rotate);
    return obj;
}


void SEllipse::draw(SCanvas& canvas) {
    cairo_t* cairo = canvas.cairo();
    cairo_save(cairo);
    cairo_translate(cairo, _center.x(), _center.y());
    cairo_rotate(cairo, _rotate * M_PI / 180.0);
    cairo_new_sub_path(cairo);
    double scale = _xradius / _yradius;
    cairo_scale(cairo, scale, 1);
    cairo_arc(cairo, 0, 0, _yradius, 0, 2 * M_PI);
    cairo_close_path(cairo);


    cairo_restore(cairo);
}



size_t SEllipse::memSize() {
    size_t len = 0;
    len += sizeof(_type);
    len += _center.memSize();
    len += sizeof(_xradius);
    len += sizeof(_yradius);
    len += sizeof(_rotate);
    return len;
}




unsigned char* SEllipse::serialize(unsigned char* data) {
    unsigned char* p = data;
    SERIALIZE(p, _type);
    p = _center.serialize(p);
    SERIALIZE(p, _xradius);
    SERIALIZE(p, _yradius);
    SERIALIZE(p, _rotate);
    return p;
}

unsigned char* SEllipse::deserialize(unsigned char* data) {
    unsigned char* p = data;
    DESERIALIZE(p, _type);
    p = _center.deserialize(p);
    DESERIALIZE(p, _xradius);
    DESERIALIZE(p, _yradius);
    DESERIALIZE(p, _rotate);
    return p;
}
