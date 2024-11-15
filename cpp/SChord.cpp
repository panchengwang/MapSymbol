#include "SChord.h"
#include "JsonHelper.h"
#include "SCanvas.h"
#include <cairo.h>
#include <math.h>



SChord::SChord()
{
    _type = SSubPath::PIE;
}

SChord::~SChord()
{

}

bool SChord::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    bool ret = SArc::fromJsonObject(obj, errorMessage);
    _type = SSubPath::CHORD;
    return ret;
}

json_object* SChord::toJsonObject()
{
    json_object* obj = SArc::toJsonObject();
    JSON_ADD_STRING(obj, "type", "chord");
    return obj;
}



void SChord::draw(SCanvas& canvas) {
    cairo_t* cairo = canvas.cairo();
    cairo_save(cairo);
    cairo_translate(cairo, _center.x(), _center.y());
    cairo_rotate(cairo, _rotate * M_PI / 180.0);
    cairo_new_sub_path(cairo);
    double scale = _xradius / _yradius;
    cairo_scale(cairo, scale, 1);
    cairo_arc(cairo, 0, 0, _yradius, _startAngle * M_PI / 180.0, _endAngle * M_PI / 180.0);
    cairo_close_path(cairo);
    cairo_restore(cairo);
}
