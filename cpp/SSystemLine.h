#ifndef __SYM_SSYSTEM_LINE_H
#define __SYM_SSYSTEM_LINE_H


#include "SShape.h"
#include "SStroke.h"
#include "DLLExport.h"


class DLL_EXPORT SSystemLine : public SShape {
public:
    SSystemLine();
    virtual ~SSystemLine();

    virtual bool fromJsonObject(json_object* obj, std::string& errorMessage);
    virtual json_object* toJsonObject();
    virtual SRect getMBR();
    virtual double getMaxStrokeWidth();

    void draw(SCanvas& canvas);
    void clear();
    size_t memSize();
    unsigned char* serialize(unsigned char* data);
    unsigned char* deserialize(unsigned char* data);

    SShape* clone();
    SStroke* stroke() const;

protected:
    SStroke* _stroke;
};

#endif
