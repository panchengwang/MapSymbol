#ifndef __SYM_SCHORD_H
#define __SYM_SCHORD_H

#include "SArc.h"
#include "DLLExport.h"



class DLL_EXPORT SChord : public SArc
{
public:
    SChord();
    virtual ~SChord();

    bool fromJsonObject(json_object* obj, std::string& errorMessage);
    json_object* toJsonObject();

    void draw(SCanvas& canvas);

    SSubPath* clone();
};

#endif // SChord_H
