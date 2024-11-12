#ifndef __SPATH_H
#define __SPATH_H


#include "SShape.h"
#include "SStroke.h"
#include "SFill.h"
#include "SSubPath.h"
#include <vector>
#include "SRect.h"

class SCanvas;

class SPath : public SShape {
public:
    SPath();
    virtual ~SPath();

    virtual bool fromJsonObject(json_object* obj, std::string& errorMessage);
    virtual json_object* toJsonObject();
    virtual SRect getMBR();
    virtual void draw(SCanvas& canvas);
    void clear();

protected:
    bool fromJsonObjectInternal(json_object* obj, std::string& errorMessage);
protected:
    SStroke* _stroke;
    SFill* _fill;
    std::vector<SSubPath*>  _subPaths;
};

#endif
