#include "SStar.h"
#include "JsonHelper.h"
#include <math.h>
#include "SCanvas.h"
#include <cairo.h>
#include "SerializeHelper.h"





SStar::SStar()
{
    _type = SSubPath::STAR;
}

SStar::~SStar()
{

}

bool SStar::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    _type = SSubPath::STAR;

    JSON_GET_POINT(obj, "center", _center, errorMessage);
    JSON_GET_DOUBLE(obj, "innerradius", _innerRadius, errorMessage);
    JSON_GET_DOUBLE(obj, "outerradius", _outerRadius, errorMessage);
    JSON_GET_DOUBLE(obj, "rotate", _rotate, errorMessage);
    JSON_GET_INT(obj, "numedges", _numEdges, errorMessage);
    return true;
}

json_object* SStar::toJsonObject()
{
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "star");
    JSON_ADD_POINT(obj, "center", _center);
    JSON_ADD_DOUBLE(obj, "innerradius", _innerRadius);
    JSON_ADD_DOUBLE(obj, "outerradius", _outerRadius);
    JSON_ADD_DOUBLE(obj, "rotate", _rotate);
    JSON_ADD_INT(obj, "numedges", _numEdges);
    return obj;
}


void SStar::draw(SCanvas& canvas) {
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
    double step = -M_PI / _numEdges;

    cairo_new_sub_path(cairo);
    cairo_move_to(cairo, _outerRadius, 0);
    for (size_t i = 0; i < _numEdges; i++) {
        cairo_line_to(cairo, _outerRadius * cos(2 * i * step), _outerRadius * sin(2 * i * step));
        cairo_line_to(cairo, _innerRadius * cos(2 * i * step + step), _innerRadius * sin(2 * i * step + step));
    }

    cairo_close_path(cairo);


    cairo_restore(cairo);

}



size_t SStar::memSize() {
    size_t len = 0;
    len += sizeof(_type);
    len += _center.memSize();
    len += sizeof(_outerRadius);
    len += sizeof(_innerRadius);
    len += sizeof(_rotate);
    len += sizeof(_numEdges);

    return len;
}




unsigned char* SStar::serialize(unsigned char* data) {
    unsigned char* p = data;
    SERIALIZE(p, _type);
    p = _center.serialize(p);
    SERIALIZE(p, _outerRadius);
    SERIALIZE(p, _innerRadius);
    SERIALIZE(p, _rotate);
    SERIALIZE(p, _numEdges);
    return p;
}

unsigned char* SStar::deserialize(unsigned char* data) {
    unsigned char* p = data;
    DESERIALIZE(p, _type);
    p = _center.deserialize(p);
    DESERIALIZE(p, _outerRadius);
    DESERIALIZE(p, _innerRadius);
    DESERIALIZE(p, _rotate);
    DESERIALIZE(p, _numEdges);
    return p;
}

SSubPath* SStar::clone()
{
    SStar *star = new SStar();
    star->_center = _center;
    star->_outerRadius = _outerRadius;
    star->_innerRadius = _innerRadius;
    star->_rotate = _rotate;
    star->_numEdges = _numEdges;
    return star;
}
