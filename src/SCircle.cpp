#include "SCircle.h"
#include "JsonHelper.h"
#include <math.h>
#include "SCanvas.h"
#include <cairo.h>



SCircle::SCircle()
{
    _type = SSubPath::CIRCLE;
}

SCircle::~SCircle()
{

}

bool SCircle::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    JSON_GET_POINT(obj, "center", _center, errorMessage);
    JSON_GET_DOUBLE(obj, "radius", _radius, errorMessage);
    return true;
}

json_object* SCircle::toJsonObject()
{
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "circle");
    JSON_ADD_POINT(obj, "center", _center);
    JSON_ADD_DOUBLE(obj, "radius", _radius);
    return obj;
}

const SPoint& SCircle::center() const
{
    return _center;
}

double SCircle::radius() const
{
    return _radius;
}


void SCircle::draw(SCanvas& canvas) {
    cairo_t* cairo = canvas.cairo();
    cairo_save(cairo);
    cairo_translate(cairo, _center.x(), _center.y());


    cairo_new_sub_path(cairo);
    cairo_arc(cairo, 0, 0, _radius, 0, 2 * M_PI);
    cairo_close_path(cairo);


    cairo_restore(cairo);
}