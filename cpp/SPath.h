#ifndef __SYM_SPATH_H
#define __SYM_SPATH_H


#include "SShape.h"
#include "SStroke.h"
#include "SFill.h"
#include "SSubPath.h"
#include <vector>
#include "SRect.h"
#include "DLLExport.h"


class SCanvas;

class DLL_EXPORT SPath : public SShape {
public:
    SPath();
    virtual ~SPath();

    virtual bool fromJsonObject(json_object* obj, std::string& errorMessage);
    virtual json_object* toJsonObject();
    virtual SRect getMBR();
    virtual void draw(SCanvas& canvas);
    void clear();
    size_t memSize();
    unsigned char* serialize(unsigned char* data);
    unsigned char* deserialize(unsigned char* data);
    SShape* clone();
protected:
    bool fromJsonObjectInternal(json_object* obj, std::string& errorMessage);
protected:
    SStroke* _stroke;
    SFill* _fill;
    std::vector<SSubPath*>  _subPaths;
};

#endif
