#include "SArc3Points.h"
#include "JsonHelper.h"
#include <math.h>
#include <string>
#include <cairo.h>
#include "SCanvas.h"
#include "SerializeHelper.h"


SArc3Points::SArc3Points()
{
    _type = SSubPath::ARC;
}

SArc3Points::~SArc3Points()
{

}

bool SArc3Points::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    _type = SSubPath::ARC_3_POINTS;

    JSON_GET_POINT(obj, "begin", _begin, errorMessage);
    JSON_GET_POINT(obj, "middle", _middle, errorMessage);
    JSON_GET_POINT(obj, "end", _end, errorMessage);

    SPoint center;
    double radius;
    if (!calculateCircleParameters(center, radius)) {
        errorMessage = "Can not calculate circle parameter from 3 points. Is the 3 points on the same line ?";
        return false;
    }
    return true;
}

json_object* SArc3Points::toJsonObject()
{
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "arc3points");
    JSON_ADD_POINT(obj, "begin", _begin);
    JSON_ADD_POINT(obj, "middle", _middle);
    JSON_ADD_POINT(obj, "end", _end);
    return obj;
}


bool  SArc3Points::calculateCircleParameters(
    SPoint& center,
    double& radius
) {

    double x1 = _begin.x(), x2 = _middle.x(), x3 = _end.x();
    double y1 = _begin.y(), y2 = _middle.y(), y3 = _end.y();
    double a = x1 - x2;
    double b = y1 - y2;
    double c = x1 - x3;
    double d = y1 - y3;
    double e = ((x1 * x1 - x2 * x2) + (y1 * y1 - y2 * y2)) * 0.5;
    double f = ((x1 * x1 - x3 * x3) + (y1 * y1 - y3 * y3)) * 0.5;
    double det = b * c - a * d;
    if (fabs(det) < 1e-10) {
        radius = -1;
        return false;
    }

    double x0 = -(d * e - b * f) / det;
    double y0 = -(a * f - c * e) / det;
    radius = hypot(x1 - x0, y1 - y0);
    center = SPoint(x0, y0);
    return true;
}


double SArc3Points::angleBetweenHorizontal(const SPoint& pt, const SPoint& center) {
    double x = pt.x() - center.x();
    double y = pt.y() - center.y();
    double angle = acos((pt.x() - center.x()) / hypot(pt.x() - center.x(), pt.y() - center.y()));
    if (y < 0) {
        angle = 2 * M_PI - angle;
    }
    return angle;
}


double SArc3Points::areaOf3Points(const SPoint& begin,
    const SPoint& middle,
    const SPoint& end) {
    SPoint points[4];
    points[0] = begin;
    points[1] = middle;
    points[2] = end;
    points[3] = begin;

    double area = 0.0;
    for (size_t i = 0; i < 3; i++) {
        area += (points[i].y() + points[i + 1].y()) * (points[i].x() - points[i + 1].x()) * 0.5;
    }
    return area;

}

bool SArc3Points::isClockwise(const SPoint& begin, const SPoint& middle, const SPoint& end) {
    if (areaOf3Points(begin, middle, end) > 0) {
        return false;
    }
    return true;
}


void SArc3Points::draw(SCanvas& canvas) {
    cairo_t* cairo = canvas.cairo();

    double radius = 0.0f;
    SPoint center;
    if (!calculateCircleParameters(center, radius)) {
        return;
    }

    double angle1, angle2, angle3;
    angle1 = angleBetweenHorizontal(_begin, center);
    // angle2 = sym_point_angle_between_horizontal(subpath->middle, center);
    angle3 = angleBetweenHorizontal(_end, center);

    bool clockwise = isClockwise(_begin, _middle, _end);

    cairo_save(cairo);

    if (clockwise) {
        cairo_move_to(cairo, _begin.x(), _begin.y());
        cairo_arc_negative(cairo, center.x(), center.y(), radius, angle1, angle3);
    }
    else {
        cairo_move_to(cairo, _begin.x(), _begin.y());
        cairo_arc(cairo, center.x(), center.y(), radius, angle1, angle3);
    }

    cairo_restore(cairo);
}



size_t SArc3Points::memSize() {
    size_t len = 0;
    len += sizeof(_type);
    len += _begin.memSize();
    len += _middle.memSize();
    len += _end.memSize();
    return len;
}



unsigned char* SArc3Points::serialize(unsigned char* data) {
    unsigned char* p = data;
    SERIALIZE(p, _type);
    p = _begin.serialize(p);
    p = _middle.serialize(p);
    p = _end.serialize(p);
    return p;
}

unsigned char* SArc3Points::deserialize(unsigned char* data) {
    unsigned char* p = data;
    DESERIALIZE(p, _type);
    p = _begin.deserialize(p);
    p = _middle.deserialize(p);
    p = _end.deserialize(p);
    return p;
}

SSubPath* SArc3Points::clone()
{
    SArc3Points *arc3pts = new SArc3Points();
    arc3pts->_type = _type;
    arc3pts->_begin = _begin;
    arc3pts->_middle = _middle;
    arc3pts->_end = _end;
    return arc3pts;
}
