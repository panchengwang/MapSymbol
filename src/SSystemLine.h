#ifndef __SSYSTEM_LINE_H
#define __SSYSTEM_LINE_H


#include "SShape.h"
#include "SStroke.h"

class SSystemLine : public SShape {
public:
    SSystemLine();
    virtual ~SSystemLine();

    virtual bool fromJsonObject(json_object* obj, std::string& errorMessage);
    virtual json_object* toJsonObject();
    virtual SRect getMBR();

protected:
    SStroke* _stroke;
};

#endif
