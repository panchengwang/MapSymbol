#include "SShape.h"
#include "SSystemFill.h"




SSystemFill::SSystemFill()
{
    _type = SShape::SYSTEM_FILL;
}

SSystemFill::~SSystemFill()
{

}

bool SSystemFill::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    return true;
}

json_object* SSystemFill::toJsonObject()
{
    return NULL;
}


SRect SSystemFill::getMBR() {
    return SRect(-1, -1, 1, 1);
}