#ifndef __SYM_SARC3POINTS_H
#define __SYM_SARC3POINTS_H

#include "SSubPath.h"
#include <string>
#include "SPoint.h"


class SArc3Points : public SSubPath
{
public:
    SArc3Points();
    virtual ~SArc3Points();
    bool fromJsonObject(json_object* obj, std::string& errorMessage);
    json_object* toJsonObject();
    void draw(SCanvas& canvas);
    size_t memSize();
    unsigned char* serialize(unsigned char* data);
    unsigned char* deserialize(unsigned char* data);
    SSubPath* clone();

protected:
    bool  calculateCircleParameters(SPoint& center, double& radius);
    double angleBetweenHorizontal(const SPoint& pt, const SPoint& center);
    double areaOf3Points(const SPoint& begin, const SPoint& middle, const SPoint& end);
    bool isClockwise(const SPoint& begin, const SPoint& middle, const SPoint& end);
protected:
    SPoint _begin, _middle, _end,_offset;
    double _rotate;
};

#endif // SARC_H
