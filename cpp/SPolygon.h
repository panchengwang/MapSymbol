#ifndef __SYM_POLYGON_H
#define __SYM_POLYGON_H

#include "SSubPath.h"
#include <vector>
#include "SLineString.h"



class SPolygon : public SLineString
{
public:
    SPolygon();
    virtual ~SPolygon();

    bool fromJsonObject(json_object* obj, std::string& errorMessage);
    json_object* toJsonObject();
    void draw(SCanvas& canvas);

    SSubPath* clone();
};

#endif // SLineString_H
