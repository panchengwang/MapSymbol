#ifndef __SCHORD_H
#define __SCHORD_H

#include "SArc.h"

class SChord : public SArc
{
public:
    SChord();
    virtual ~SChord();

    bool fromJsonObject(json_object* obj, std::string& errorMessage)   ;
    json_object* toJsonObject()   ;
};

#endif // SChord_H
