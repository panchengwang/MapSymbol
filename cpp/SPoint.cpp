#include "SPoint.h"
#include "JsonHelper.h"
#include <math.h>
#include <iostream>
#include "SerializeHelper.h"


SPoint::SPoint() {
    _x = _y = 0.0f;
}

SPoint::SPoint(double x, double y) {
    _x = x;
    _y = y;
}

SPoint::SPoint(const SPoint& pt) {
    _x = pt._x;
    _y = pt._y;
}

SPoint::~SPoint() {

}

json_object* SPoint::toJsonObject() const {
    json_object* obj = json_object_new_object();
    JSON_ADD_DOUBLE(obj, "x", _x);
    JSON_ADD_DOUBLE(obj, "y", _y);
    return obj;
}


bool SPoint::fromJsonObject(json_object* obj, std::string& errorMessage) {
    JSON_GET_DOUBLE(obj, "x", _x, errorMessage);
    JSON_GET_DOUBLE(obj, "y", _y, errorMessage);
    return true;
}


SRect SPoint::getMBR() {
    return SRect(_x, _y, _x, _y);
}


double SPoint::x() const {
    return _x;
}


double SPoint::y() const {
    return _y;
}


bool SPoint::equal(const SPoint& other) {
    double dist = (_x - other._x) * (_x - other._x) + (_y - other._y) * (_y - other._y);
    std::cout << "distance: " << dist << std::endl;
    return dist <= 0.00000000001;
}


std::string SPoint::toString() {
    char buf[256];
    sprintf(buf, "POINT(%f,%f)", _x, _y);
    return buf;
}


size_t SPoint::memSize() {
    size_t len = 0;
    len += sizeof(_x);
    len += sizeof(_y);
    return len;
}


unsigned char* SPoint::serialize(unsigned char* data) {
    unsigned char* p = data;
    SERIALIZE(p, _x);
    SERIALIZE(p, _y);
    return p;
}

unsigned char* SPoint::deserialize(unsigned char* data) {
    unsigned char* p = data;
    DESERIALIZE(p, _x);
    DESERIALIZE(p, _y);
    return p;
}
