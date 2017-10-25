#pragma once

namespace TS
{
    class Object
    {
    public:
        const char*  GetClassName() const;
        virtual const char* ToString()const;
        operator const char*() const;
        virtual ~Object();
    };
}

