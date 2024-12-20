#ifndef __SYM_SPIE_H
#define __SYM_SPIE_H

#include "SArc.h"
#include "DLLExport.h"


class DLL_EXPORT SPie : public SArc
{
public:
    SPie();
    virtual ~SPie();

    bool fromJsonObject(json_object* obj, std::string& errorMessage);
    json_object* toJsonObject();
    void draw(SCanvas& canvas);

    SSubPath* clone();
};

#endif // SPIE_H
