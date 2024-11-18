#ifndef __SYM_SSYSTEM_FILL_H
#define __SYM_SSYSTEM_FILL_H


#include "SFill.h"

#include "SShape.h"

class SSystemFill : public SShape {
public:
    SSystemFill();
    virtual ~SSystemFill();

    virtual bool fromJsonObject(json_object* obj, std::string& errorMessage);
    virtual json_object* toJsonObject();
    virtual SRect getMBR();
    void clear();
    void draw(SCanvas& canvas);

    size_t memSize();
    unsigned char* serialize(unsigned char* data);
    unsigned char* deserialize(unsigned char* data);
    SShape* clone();
protected:
    SFill* _fill;
};




#endif
