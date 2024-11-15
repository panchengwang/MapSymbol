#include "SPie.h"
#include "JsonHelper.h"
#include <cairo.h>
#include <math.h>
#include "SCanvas.h"



SPie::SPie()
{
    _type = SSubPath::PIE;
}

SPie::~SPie()
{

}

bool SPie::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    bool ret = SArc::fromJsonObject(obj, errorMessage);
    _type = SSubPath::PIE;
    return ret;
}

json_object* SPie::toJsonObject()
{
    json_object* obj = SArc::toJsonObject();
    JSON_ADD_STRING(obj, "type", "pie");
    return obj;
}



void SPie::draw(SCanvas& canvas) {
    cairo_t* cairo = canvas.cairo();
    cairo_save(cairo);
    cairo_translate(cairo, _center.x(), _center.y());
    cairo_rotate(cairo, _rotate * M_PI / 180.0);
    cairo_new_sub_path(cairo);
    double scale = _xradius / _yradius;
    cairo_scale(cairo, scale, 1);
    cairo_arc(cairo, 0, 0, _yradius, _startAngle * M_PI / 180.0, _endAngle * M_PI / 180.0);
    cairo_line_to(cairo, 0, 0);
    cairo_close_path(cairo);


    cairo_restore(cairo);
}


