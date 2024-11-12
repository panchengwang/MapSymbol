#include "SPoint.h"
#include "JsonHelper.h"


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


double SPoint::x() {
    return _x;
}


double SPoint::y() {
    return _y;
}

