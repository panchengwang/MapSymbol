#ifndef __SYM_SFILL_H
#define __SYM_SFILL_H

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
    virtual size_t memSize() = 0;
    virtual unsigned char* serialize(unsigned char* data) = 0;
    virtual unsigned char* deserialize(unsigned char* data) = 0;
public:
    uint8_t  _type;

};

#endif
