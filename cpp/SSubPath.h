#ifndef __SYM_SSUBPATH_H
#define __SYM_SSUBPATH_H

#include <json.h>
#include <string>

class SCanvas;
class SSubPath
{
public:
    enum {
        CIRCLE = 1,
        ELLIPSE,
        ARC_3_POINTS,
        ARC,
        PIE,
        CHORD,
        LINESTRING,
        POLYGON,
        REGULAR_POLYGON,
        STAR,
        TEXT
    };
public:
    SSubPath();
    virtual ~SSubPath();

    virtual bool fromJsonObject(json_object* obj, std::string& errorMessage) = 0;
    virtual json_object* toJsonObject() = 0;
    virtual size_t memSize() = 0;
    uint8_t type() const;

    virtual void draw(SCanvas& canvas) = 0;
    virtual unsigned char* serialize(unsigned char* data) = 0;
    virtual unsigned char* deserialize(unsigned char* data) = 0;
protected:
    uint8_t _type;
};

#endif // SSUBPATH_H
