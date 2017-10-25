#pragma once
#include "Object.h"
#include "Interface.h"

namespace TS
{
    class INamed : public Interface
                  ,public Object
    {
    private:
        char m_iname[1024];
    public:        
        const char * GetIName()const;       
        bool SetIName(const char *);  

        const char* ToString() const override ;
        
        INamed();
    };

}
