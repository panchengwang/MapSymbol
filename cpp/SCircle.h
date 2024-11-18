#ifndef __SYM_SCIRCLE_H
#define __SYM_SCIRCLE_H

#include "SSubPath.h"
#include <string>
#include "SPoint.h"


class SCircle : public SSubPath
{
public:
    SCircle();
    virtual ~SCircle();

    bool fromJsonObject(json_object* obj, std::string& errorMessage);
    json_object* toJsonObject();

    const SPoint& center() const;

    double radius() const;
    void draw(SCanvas& canvas);
    size_t memSize();
    unsigned char* serialize(unsigned char* data);
    unsigned char* deserialize(unsigned char* data);

    SSubPath* clone();
protected:
    SPoint _center;
    double _radius;
};

#endif // SCIRCLE_H
