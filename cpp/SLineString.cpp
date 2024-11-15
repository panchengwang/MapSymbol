#include "SLineString.h"
#include "JsonHelper.h"
#include <cairo.h>
#include <math.h>
#include "SCanvas.h"
#include "SerializeHelper.h"


SLineString::SLineString()
{
    _type = SSubPath::LINESTRING;
}

SLineString::~SLineString()
{

}

bool SLineString::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    _points.clear();

    _type = SSubPath::LINESTRING;
    json_object* ptobjarr;
    JSON_GET_OBJ(obj, "points", ptobjarr, errorMessage);
    size_t npts = json_object_array_length(ptobjarr);
    if (npts < 2) {
        errorMessage = "Invalid linestring: < 2 points ";
        return false;
    }
    for (size_t i = 0; i < npts; i++) {
        json_object* ptobj = json_object_array_get_idx(ptobjarr, i);
        SPoint pt;
        if (!pt.fromJsonObject(ptobj, errorMessage)) {
            return false;
        }
        _points.push_back(pt);
    }



    return true;
}

json_object* SLineString::toJsonObject()
{
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "linestring");
    json_object* ptobjarr = json_object_new_array();
    for (size_t i = 0; i < _points.size(); i++) {
        json_object_array_add(
            ptobjarr,
            _points[i].toJsonObject()
        );
    }
    JSON_ADD_OBJ(obj, "points", ptobjarr);
    return obj;
}



void SLineString::draw(SCanvas& canvas) {
    cairo_t* cairo = canvas.cairo();
    cairo_save(cairo);

    cairo_new_sub_path(cairo);
    cairo_move_to(cairo, _points[0].x(), _points[0].y());
    for (size_t i = 1; i < _points.size(); i++) {
        cairo_line_to(cairo, _points[i].x(), _points[i].y());
    }


    cairo_restore(cairo);
}



size_t SLineString::memSize() {
    size_t len = 0;
    len += sizeof(_type);
    size_t npoints = _points.size();
    len += sizeof(npoints);
    for (size_t i = 0; i < npoints; i++) {
        len += _points[i].memSize();
    }
    return len;
}



unsigned char* SLineString::serialize(unsigned char* data) {
    unsigned char* p = data;
    SERIALIZE(p, _type);
    size_t npoints = _points.size();
    SERIALIZE(p, npoints);
    for (size_t i = 0; i < npoints; i++) {
        p = _points[i].serialize(p);
    }
    return p;
}

unsigned char* SLineString::deserialize(unsigned char* data) {
    _points.clear();

    unsigned char* p = data;
    DESERIALIZE(p, _type);
    size_t npoints;
    DESERIALIZE(p, npoints);
    for (size_t i = 0; i < npoints; i++) {
        SPoint pt;
        p = pt.deserialize(p);
        _points.push_back(pt);
    }
    return p;
}
