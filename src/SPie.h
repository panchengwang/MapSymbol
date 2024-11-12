#ifndef SPIE_H
#define SPIE_H

#include "SArc.h"

class SPie : public SArc
{
public:
    SPie();
    virtual ~SPie();

    bool fromJsonObject(json_object* obj, std::string& errorMessage)   ;
    json_object* toJsonObject()   ;

};

#endif // SPIE_H
