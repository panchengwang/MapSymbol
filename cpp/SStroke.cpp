#include "SStroke.h"
#include "JsonHelper.h"
#include <iostream>
#include <cairo.h>
#include "SCanvas.h"
#include "SerializeHelper.h"


SStroke::SStroke()
{
    // _ndashes = 0;
    // _dashes = NULL;
}

SStroke::~SStroke()
{
    clear();
}

bool SStroke::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    bool ret = fromJsonObjectInternal(obj, errorMessage);
    if (!ret) {
        clear();
    }
    return ret;
}

json_object* SStroke::toJsonObject()
{
    json_object* obj = json_object_new_object();
    std::string str;
    if (_cap == SStroke::CAP_BUTT) {
        str = "butt";
    }
    else if (_cap == SStroke::CAP_ROUND) {
        str = "round";
    }
    else if (_cap == SStroke::CAP_SQUARE) {
        str = "square";
    }
    JSON_ADD_STRING(obj, "cap", str.c_str());

    if (_join == SStroke::JOIN_MITER) {
        str = "miter";
    }
    else if (_join == SStroke::JOIN_ROUND) {
        str = "round";
    }
    else if (_join == SStroke::JOIN_BEVEL) {
        str = "bevel";
    }
    JSON_ADD_STRING(obj, "join", str.c_str());
    JSON_ADD_DOUBLE(obj, "miter", _miter);

    JSON_ADD_OBJ(obj, "color", _color.toJsonObject());
    JSON_ADD_DOUBLE(obj, "width", _width);
    JSON_ADD_DOUBLE(obj, "dashoffset", _dashOffset);

    json_object* dashesobjarr = json_object_new_array();
    // for (size_t i = 0; i < _ndashes; i++) {
    //     json_object_array_add(dashesobjarr, json_object_new_double(_dashes[i]));
    // }
    for (size_t i = 0; i < _dashes.size(); i++) {
        json_object_array_add(dashesobjarr, json_object_new_double(_dashes[i]));
    }
    JSON_ADD_OBJ(obj, "dashes", dashesobjarr);
    return obj;
}

void SStroke::clear()
{
    // if (_dashes) {
    //     delete[] _dashes;
    // }
    // _dashes = NULL;
    // _ndashes = 0;

    _dashes.clear();
}

double SStroke::miter() const
{
    return _miter;
}


void SStroke::setTo(SCanvas& canvas) {
    cairo_t* cairo = canvas.cairo();

    cairo_set_line_width(cairo, width() / canvas.xScale() * canvas.dotsPerMM());
    if (_cap == SStroke::CAP_BUTT) {
        cairo_set_line_cap(cairo, CAIRO_LINE_CAP_BUTT);
    }
    else if (_cap == SStroke::CAP_ROUND) {
        cairo_set_line_cap(cairo, CAIRO_LINE_CAP_ROUND);
    }
    else if (_cap == SStroke::CAP_SQUARE) {
        cairo_set_line_cap(cairo, CAIRO_LINE_CAP_SQUARE);
    }

    if (_join == SStroke::JOIN_BEVEL) {
        cairo_set_line_join(cairo, CAIRO_LINE_JOIN_BEVEL);
    }
    else if (_join == SStroke::JOIN_MITER) {
        cairo_set_line_join(cairo, CAIRO_LINE_JOIN_MITER);
        cairo_set_miter_limit(cairo, _miter);
    }
    else if (_join == SStroke::JOIN_ROUND) {
        cairo_set_line_join(cairo, CAIRO_LINE_JOIN_ROUND);
    }


    cairo_set_source_rgba(cairo,
        _color.red() / 255.0,
        _color.green() / 255.0,
        _color.blue() / 255.0,
        _color.alpha() / 255.0);

    double* dashes = new double[_dashes.size()];
    for (size_t i = 0; i < _dashes.size(); i++) {
        dashes[i] = _dashes[i] / canvas.xScale() * canvas.dotsPerMM();
    }

    cairo_set_dash(cairo, dashes, _dashes.size(),
        _dashOffset / canvas.xScale() * canvas.dotsPerMM());
}


bool SStroke::fromJsonObjectInternal(json_object* obj, std::string& errorMessage)
{
    std::string str;
    JSON_GET_STRING(obj, "cap", str, errorMessage);
    if (str == "butt") {
        _cap = SStroke::CAP_BUTT;
    }
    else if (str == "round") {
        _cap = SStroke::CAP_ROUND;
    }
    else if (str == "square") {
        _cap = SStroke::CAP_SQUARE;
    }
    else {
        errorMessage = "Invalid line cap : " + str;
        return false;
    }

    JSON_GET_STRING(obj, "join", str, errorMessage);
    if (str == "miter") {
        _join = SStroke::JOIN_MITER;
    }
    else if (str == "round") {
        _join = SStroke::JOIN_ROUND;
    }
    else if (str == "bevel") {
        _join = SStroke::JOIN_BEVEL;
    }
    else {
        errorMessage = "Invalid line join: " + str;
        return false;
    }

    JSON_GET_DOUBLE(obj, "miter", _miter, errorMessage);

    json_object* colorobj = NULL;
    JSON_GET_OBJ(obj, "color", colorobj, errorMessage);
    if (!_color.fromJsonObject(colorobj, errorMessage)) {
        return false;
    }

    JSON_GET_DOUBLE(obj, "width", _width, errorMessage);

    JSON_GET_DOUBLE(obj, "dashoffset", _dashOffset, errorMessage);

    json_object* dashesobjarr = NULL;
    JSON_GET_OBJ(obj, "dashes", dashesobjarr, errorMessage);
    size_t ndashes = json_object_array_length(dashesobjarr);
    clear();
    for (size_t i = 0; i < ndashes; i++) {
        json_object* dashobj = json_object_array_get_idx(dashesobjarr, i);
        _dashes.push_back(json_object_get_double(dashobj));
    }
    // if (_dashes) {
    //     delete[] _dashes;
    // }
    // _dashes = new double[_ndashes];
    // for (size_t i = 0; i < _ndashes; i++) {
    //     json_object* dashobj = json_object_array_get_idx(dashesobjarr, i);
    //     _dashes[i] = json_object_get_double(dashobj);
    // }

    return true;
}

uint8_t SStroke::join() const
{
    return _join;
}

uint8_t SStroke::cap() const
{
    return _cap;
}

SColor SStroke::color() const
{
    return _color;
}

double SStroke::width() const
{
    return _width;
}



size_t SStroke::memSize() {
    size_t len = 0;
    len += sizeof(_width);
    len += _color.memSize();
    len += sizeof(_cap);
    len += sizeof(_join);
    len += sizeof(_miter);
    len += sizeof(_dashOffset);
    size_t ndashes = _dashes.size();
    len += sizeof(ndashes);
    len += ndashes * sizeof(double);
    return len;
}



unsigned char* SStroke::serialize(unsigned char* data) {
    unsigned char* p = data;
    SERIALIZE(p, _width);
    p = _color.serialize(p);
    SERIALIZE(p, _cap);
    SERIALIZE(p, _join);
    SERIALIZE(p, _miter);
    SERIALIZE(p, _dashOffset);
    size_t ndashes = _dashes.size();
    SERIALIZE(p, ndashes);
    for (size_t i = 0; i < ndashes; i++) {
        // double d = _dashes[i];
        SERIALIZE(p, _dashes[i]);
    }
    return p;
}

unsigned char* SStroke::deserialize(unsigned char* data) {
    _dashes.clear();
    unsigned char* p = data;
    DESERIALIZE(p, _width);
    p = _color.deserialize(p);
    DESERIALIZE(p, _cap);
    DESERIALIZE(p, _join);
    DESERIALIZE(p, _miter);
    DESERIALIZE(p, _dashOffset);
    size_t ndashes;
    DESERIALIZE(p, ndashes);
    for (size_t i = 0; i < ndashes; i++) {
        double d;
        DESERIALIZE(p, d);
        _dashes.push_back(d);
    }
    return p;
}
