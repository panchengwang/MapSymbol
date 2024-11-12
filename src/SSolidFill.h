#ifndef SSOLIDFILL_H
#define SSOLIDFILL_H

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

protected:
    SColor  _color;

};

#endif // SSOLIDFILL_H
