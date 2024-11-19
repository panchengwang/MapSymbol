#include "SRegularPolygon.h"
#include "JsonHelper.h"
#include "SCanvas.h"
#include <cairo.h>
#include <math.h>
#include "SerializeHelper.h"





SRegularPolygon::SRegularPolygon()
{
    _type = SSubPath::REGULAR_POLYGON;
}

SRegularPolygon::~SRegularPolygon()
{

}

bool SRegularPolygon::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    _type = SSubPath::REGULAR_POLYGON;

    JSON_GET_POINT(obj, "center", _center, errorMessage);
    JSON_GET_DOUBLE(obj, "radius", _radius, errorMessage);
    JSON_GET_DOUBLE(obj, "rotate", _rotate, errorMessage);
    JSON_GET_INT(obj, "numedges", _numEdges, errorMessage);
    return true;
}

json_object* SRegularPolygon::toJsonObject()
{
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "regularpolygon");
    JSON_ADD_POINT(obj, "center", _center);
    JSON_ADD_DOUBLE(obj, "radius", _radius);
    JSON_ADD_DOUBLE(obj, "rotate", _rotate);
    JSON_ADD_INT(obj, "numedges", _numEdges);
    return obj;
}


void SRegularPolygon::draw(SCanvas& canvas) {
    cairo_t* cairo = canvas.cairo();
    cairo_save(cairo);
    cairo_translate(cairo, _center.x(), _center.y());
    cairo_rotate(cairo, _rotate / 180.0 * M_PI);
    double rotateangle = 0.0;
    if (_numEdges % 2 == 1) {
        rotateangle = M_PI_2;
    }
    else if (_numEdges % 4 == 0) {
        rotateangle = M_PI / _numEdges;
    }
    cairo_rotate(cairo, rotateangle);
    double step = 2 * M_PI / _numEdges;

    cairo_new_sub_path(cairo);
    cairo_move_to(cairo, _radius, 0);
    for (size_t i = 0; i < _numEdges; i++) {
        cairo_line_to(cairo, _radius * cos(i * step), _radius * sin(i * step));
    }
    cairo_line_to(cairo, _radius, 0);
    cairo_close_path(cairo);

    cairo_restore(cairo);

}


size_t SRegularPolygon::memSize() {
    size_t len = 0;
    len += sizeof(_type);
    len += _center.memSize();
    len += sizeof(_radius);
    len += sizeof(_rotate);
    len += sizeof(_numEdges);

    return len;
}



unsigned char* SRegularPolygon::serialize(unsigned char* data) {
    unsigned char* p = data;
    SERIALIZE(p, _type);
    p = _center.serialize(p);
    SERIALIZE(p, _radius);
    SERIALIZE(p, _rotate);
    SERIALIZE(p, _numEdges);
    return p;
}

unsigned char* SRegularPolygon::deserialize(unsigned char* data) {
    unsigned char* p = data;
    DESERIALIZE(p, _type);
    p = _center.deserialize(p);
    DESERIALIZE(p, _radius);
    DESERIALIZE(p, _rotate);
    DESERIALIZE(p, _numEdges);
    return p;
}

SSubPath* SRegularPolygon::clone()
{
    SRegularPolygon *rpg = new SRegularPolygon();
    rpg->_center = _center;
    rpg->_radius = _radius;
    rpg->_rotate = _rotate;
    rpg->_numEdges = _numEdges;
    return rpg;
}

