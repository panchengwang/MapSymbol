#ifndef __SYM_SPOINT_H
#define __SYM_SPOINT_H

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

    double x() const;
    double y() const;

    bool equal(const SPoint& other);

    std::string toString();

    size_t memSize();
    unsigned char* serialize(unsigned char* data);
    unsigned char* deserialize(unsigned char* data);
protected:
    double _x, _y;
};

#endif
