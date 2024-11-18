#include "SPolygon.h"
#include "JsonHelper.h"
#include <cairo.h>
#include <math.h>
#include "SCanvas.h"
#include <iostream>


SPolygon::SPolygon()
{
    _type = SSubPath::POLYGON;
}

SPolygon::~SPolygon()
{

}

bool SPolygon::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    bool ret = SLineString::fromJsonObject(obj, errorMessage);
    _type = SSubPath::POLYGON;
    if (!_points[0].equal(_points[_points.size() - 1])) {
        _points.push_back(_points[0]);
    }
    return ret;
}

json_object* SPolygon::toJsonObject()
{
    json_object* obj = SLineString::toJsonObject();
    JSON_ADD_STRING(obj, "type", "linestring");
    return obj;
}



void SPolygon::draw(SCanvas& canvas) {
    cairo_t* cairo = canvas.cairo();
    cairo_save(cairo);

    cairo_new_sub_path(cairo);
    cairo_move_to(cairo, _points[0].x(), _points[0].y());
    for (size_t i = 1; i < _points.size(); i++) {
        cairo_line_to(cairo, _points[i].x(), _points[i].y());
    }
    cairo_close_path(cairo);

    cairo_restore(cairo);
}

SSubPath* SPolygon::clone()
{
    SPolygon *pg = new SPolygon();
    pg->_type = _type;
    pg->_points = _points;
    return pg;
}
