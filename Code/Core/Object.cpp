#include "Object.h"
#include <typeinfo>

const char* TS::Object::GetClassName() const
{
    return typeid(*this).name();
}

const char* TS::Object::ToString() const
{
    return GetClassName();
}

TS::Object::operator const char*() const
{
    return ToString();
}

