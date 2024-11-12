#ifndef __SSTAR_H
#define __SSTAR_H

#include "SSubPath.h"
#include <string>
#include "SPoint.h"

class SStar : public SSubPath
{
public:
    SStar();
    virtual ~SStar();

    bool fromJsonObject(json_object* obj, std::string& errorMessage);
    json_object* toJsonObject();

    void draw(SCanvas& canvas);

protected:
    SPoint  _center;
    double  _outerRadius;
    double  _innerRadius;
    double  _rotate;
    size_t  _numEdges;
};

#endif // SStar_H
