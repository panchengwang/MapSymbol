#ifndef __SYM_SREGULARPOLYGON_H
#define __SYM_SREGULARPOLYGON_H

#include "SSubPath.h"
#include <string>
#include "SPoint.h"

class SRegularPolygon : public SSubPath
{
public:
    SRegularPolygon();
    virtual ~SRegularPolygon();

    bool fromJsonObject(json_object* obj, std::string& errorMessage);
    json_object* toJsonObject();

    void draw(SCanvas& canvas);
    size_t memSize();
    unsigned char* serialize(unsigned char* data);
    unsigned char* deserialize(unsigned char* data);
protected:
    SPoint  _center;
    double  _radius;
    double  _rotate;
    size_t  _numEdges;
};

#endif // SREGULARPOLYGON_H
