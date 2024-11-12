#include "SChord.h"
#include "JsonHelper.h"


SChord::SChord()
{
    _type = SSubPath::PIE;
}

SChord::~SChord()
{

}

bool SChord::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    bool ret = SArc::fromJsonObject(obj, errorMessage);
    _type = SSubPath::CHORD;
    return ret;
}

json_object* SChord::toJsonObject()
{
    json_object* obj = SArc::toJsonObject();
    JSON_ADD_STRING(obj,"type", "chord");
    return obj;
}
