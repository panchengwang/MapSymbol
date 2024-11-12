#ifndef __SREGULARPOLYGON_H
#define __SREGULARPOLYGON_H

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

protected:
    SPoint  _center;
    double  _radius;
    double  _rotate;
    size_t  _numEdges;
};

#endif // SREGULARPOLYGON_H