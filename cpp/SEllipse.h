#ifndef __SYM_SELLIPSE_H
#define __SYM_SELLIPSE_H

#include "SSubPath.h"
#include <string>
#include "SPoint.h"


class SEllipse : public SSubPath
{
public:
    SEllipse();
    virtual ~SEllipse();

    bool fromJsonObject(json_object* obj, std::string& errorMessage);
    json_object* toJsonObject();

    void draw(SCanvas& canvas);
    size_t memSize();
    unsigned char* serialize(unsigned char* data);
    unsigned char* deserialize(unsigned char* data);
protected:
    SPoint  _center;
    double  _xradius;
    double  _yradius;
    double  _rotate;

};

#endif // SEllipse_H
