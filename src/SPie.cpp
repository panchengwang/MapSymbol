#include "SPie.h"
#include "JsonHelper.h"


SPie::SPie()
{
    _type = SSubPath::PIE;
}

SPie::~SPie()
{

}

bool SPie::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    bool ret = SArc::fromJsonObject(obj, errorMessage);
    _type = SSubPath::PIE;
    return ret;
}

json_object* SPie::toJsonObject()
{
    json_object* obj = SArc::toJsonObject();
    JSON_ADD_STRING(obj,"type", "pie");
    return obj;
}
