#ifndef __SSHAPE_H
#define __SSHAPE_H

#include <json.h>
#include <string>
#include "SRect.h"

class SCanvas;
class SShape
{
public:
    enum {
        SYSTEM_LINE = 1,
        SYSTEM_FILL,
        PATH
    };

public:
    SShape();
    virtual ~SShape();

    virtual bool fromJsonObject(json_object* obj, std::string& errorMessage) = 0;
    virtual json_object* toJsonObject() = 0;
    virtual SRect getMBR() = 0;
    uint8_t type() const;

    virtual void draw(SCanvas& canvas);

protected:
    uint8_t _type;
};




#endif
