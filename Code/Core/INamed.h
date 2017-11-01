#pragma once
#include "Object.h"
#include "Develop.h"

namespace TS
{    
    
    class INamed : public Object
    {
    private:
        char m_iname[1024];

    public:        
        const char * GetIName()const;       
        bool SetIName(const char *);  

        const char* ToString() const override ;
        
        INamed();
        virtual ~INamed() = default;

        TS_DISABLE_COPY(INamed);
    };

}
