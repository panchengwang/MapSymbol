#ifndef __SYM_SSTAR_H
#define __SYM_SSTAR_H

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
    size_t memSize();

    unsigned char* serialize(unsigned char* data);
    unsigned char* deserialize(unsigned char* data);
protected:
    SPoint  _center;
    double  _outerRadius;
    double  _innerRadius;
    double  _rotate;
    size_t  _numEdges;
};

#endif // SStar_H
