#ifndef __SARC3POINTS_H
#define __SARC3POINTS_H

#include "SSubPath.h"
#include <string>
#include "SPoint.h"


class SArc3Points : public SSubPath
{
public:
    SArc3Points();
    virtual ~SArc3Points();
    bool fromJsonObject(json_object* obj, std::string& errorMessage)   ;
    json_object* toJsonObject()   ;


protected:
    SPoint _begin, _middle, _end;

};

#endif // SARC_H
