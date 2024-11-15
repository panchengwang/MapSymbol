#ifndef __SYM_SSOLIDFILL_H
#define __SYM_SSOLIDFILL_H

#include "SFill.h"
#include "SColor.h"

class SCanvas;
class SSolidFill : public SFill
{
public:
    SSolidFill();
    virtual ~SSolidFill();

    bool fromJsonObject(json_object* obj, std::string& errorMessage);
    json_object* toJsonObject();
    void setTo(SCanvas& canvas);

    size_t memSize();
    unsigned char* serialize(unsigned char* data);
    unsigned char* deserialize(unsigned char* data);
protected:
    SColor  _color;

};

#endif // SSOLIDFILL_H
