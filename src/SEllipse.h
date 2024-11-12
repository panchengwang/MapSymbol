#ifndef __SELLIPSE_H
#define __SELLIPSE_H

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

protected:
    SPoint  _center;
    double  _xradius;
    double  _yradius;
    double  _rotate;

};

#endif // SEllipse_H
