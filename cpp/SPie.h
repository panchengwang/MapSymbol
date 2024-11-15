#ifndef __SYM_SPIE_H
#define __SYM_SPIE_H

#include "SArc.h"

class SPie : public SArc
{
public:
    SPie();
    virtual ~SPie();

    bool fromJsonObject(json_object* obj, std::string& errorMessage);
    json_object* toJsonObject();
    void draw(SCanvas& canvas);
};

#endif // SPIE_H
