#ifndef __SYM_SARC_H
#define __SYM_SARC_H

#include "SSubPath.h"
#include <string>
#include "SPoint.h"

class SCanvas;

class SArc : public SSubPath
{
public:
    SArc();
    virtual ~SArc();

    bool fromJsonObject(json_object* obj, std::string& errorMessage);
    json_object* toJsonObject();
    void draw(SCanvas& canvas);

    size_t memSize();
    unsigned char* serialize(unsigned char* data);
    unsigned char* deserialize(unsigned char* data);
protected:
    SPoint  _center;
    double  _xradius, _yradius;
    double  _rotate;
    double  _startAngle;
    double  _endAngle;

};

#endif // SARC_H
