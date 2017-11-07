#include "Object.h"
#include <typeinfo>

const char* TS::Object::GetTypeName() const
{
    return typeid(*this).name();
}

const char* TS::Object::ToString() const
{
    return GetTypeName();
}

TS::Object::operator const char*() const
{
    return ToString();
}

