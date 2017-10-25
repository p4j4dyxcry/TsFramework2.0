#pragma once
#include "Object.h"

namespace TS
{
    class NamedObject : public Object
    {
    private:
        char m_name[256];
    public:
        const char * GetName()const;
        bool SetName(const char *);
        NamedObject();
    };

}
