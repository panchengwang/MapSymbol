#ifndef __SSYSTEM_FILL_H
#define __SSYSTEM_FILL_H




#include "SShape.h"

class SSystemFill : public SShape {
public:
    SSystemFill();
    virtual ~SSystemFill();

    virtual bool fromJsonObject(json_object* obj, std::string& errorMessage);
    virtual json_object* toJsonObject();
    virtual SRect getMBR();
};




#endif
