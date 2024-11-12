#include "SArc.h"
#include "JsonHelper.h"
#include "SCanvas.h"
#include <math.h>
#include <cairo.h>



SArc::SArc()
{
    _type = SSubPath::ARC;
}

SArc::~SArc()
{

}

bool SArc::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    _type = SSubPath::ARC;
    JSON_GET_POINT(obj, "center", _center, errorMessage);
    JSON_GET_DOUBLE(obj, "xradius", _xradius, errorMessage);
    JSON_GET_DOUBLE(obj, "yradius", _yradius, errorMessage);
    JSON_GET_DOUBLE(obj, "rotate", _rotate, errorMessage);
    JSON_GET_DOUBLE(obj, "startangle", _startAngle, errorMessage);
    JSON_GET_DOUBLE(obj, "endangle", _endAngle, errorMessage);
    return true;
}

json_object* SArc::toJsonObject()
{
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "arc");
    JSON_ADD_POINT(obj, "center", _center);
    JSON_ADD_DOUBLE(obj, "xradius", _xradius);
    JSON_ADD_DOUBLE(obj, "yradius", _yradius);
    JSON_ADD_DOUBLE(obj, "rotate", _rotate);
    JSON_ADD_DOUBLE(obj, "startangle", _startAngle);
    JSON_ADD_DOUBLE(obj, "endangle", _endAngle);
    return obj;
}



void SArc::draw(SCanvas& canvas) {
    cairo_t* cairo = canvas.cairo();
    cairo_save(cairo);
    cairo_translate(cairo, _center.x(), _center.y());
    cairo_rotate(cairo, _rotate * M_PI / 180.0);
    cairo_new_sub_path(cairo);
    double scale = _xradius / _yradius;
    cairo_scale(cairo, scale, 1);
    cairo_arc(cairo, 0, 0, _yradius, _startAngle * M_PI / 180.0, _endAngle * M_PI / 180.0);
    // cairo_close_path(cairo);


    cairo_restore(cairo);
}

