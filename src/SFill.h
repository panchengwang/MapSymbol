#ifndef __SFILL_H
#define __SFILL_H

#include <string>
#include <json.h>
#include <stdint.h>

class SCanvas;

class SFill {
public:
    enum {
        SOLID
    };
public:
    SFill();
    virtual ~SFill();

    virtual bool fromJsonObject(json_object* obj, std::string& errorMessage) = 0;
    virtual json_object* toJsonObject() = 0;
    uint8_t type() const;

    virtual void setTo(SCanvas& canvas);

public:
    uint8_t  _type;

};

#endif
