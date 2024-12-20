#ifndef __SYM_SSHAPE_H
#define __SYM_SSHAPE_H

#include <json.h>
#include <string>
#include "SRect.h"
#include "DLLExport.h"


class SCanvas;
class DLL_EXPORT SShape
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
    virtual double getMaxStrokeWidth();
    virtual size_t memSize() = 0;
    virtual uint8_t type() const;

    virtual void draw(SCanvas& canvas);
    virtual unsigned char* serialize(unsigned char* data) = 0;
    virtual unsigned char* deserialize(unsigned char* data) = 0;

    virtual SShape* clone() = 0;

protected:
    uint8_t _type;
};




#endif
