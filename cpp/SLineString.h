#ifndef __SYM_LINESTRING_H
#define __SYM_LINESTRING_H

#include "SSubPath.h"
#include <vector>
#include "SPoint.h"



class SLineString : public SSubPath
{
public:
    SLineString();
    virtual ~SLineString();

    bool fromJsonObject(json_object* obj, std::string& errorMessage);
    json_object* toJsonObject();
    void draw(SCanvas& canvas);
    size_t memSize();
    unsigned char* serialize(unsigned char* data);
    unsigned char* deserialize(unsigned char* data);
protected:
    std::vector<SPoint> _points;
};

#endif // SLineString_H