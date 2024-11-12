#ifndef __SPOINT_H
#define __SPOINT_H

#include <json.h>
#include <string>
#include "SRect.h"


class SPoint {
public:
    SPoint();
    SPoint(double x, double y);
    SPoint(const SPoint& pt);
    virtual ~SPoint();

    json_object* toJsonObject() const;
    bool fromJsonObject(json_object* obj, std::string& errorMessage);
    SRect getMBR();

    double x();
    double y();
protected:
    double _x, _y;
};

#endif
