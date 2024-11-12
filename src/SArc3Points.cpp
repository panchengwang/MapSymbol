#include "SArc3Points.h"
#include "JsonHelper.h"



SArc3Points::SArc3Points()
{
    _type = SSubPath::ARC;
}

SArc3Points::~SArc3Points()
{

}

bool SArc3Points::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    _type = SSubPath::ARC_3_POINTS;

    JSON_GET_POINT(obj, "begin", _begin, errorMessage);
    JSON_GET_POINT(obj, "middle", _middle, errorMessage);
    JSON_GET_POINT(obj, "end", _end, errorMessage);

    return true;
}

json_object* SArc3Points::toJsonObject()
{
    json_object *obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "arc3points");
    JSON_ADD_POINT(obj, "begin", _begin);
    JSON_ADD_POINT(obj, "middle", _middle);
    JSON_ADD_POINT(obj, "end", _end);
    return obj;
}

