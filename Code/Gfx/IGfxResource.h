#pragma once
#include "Code/Core/INamed.h"

namespace TS
{
    class IGfxResource : public INamed
    {
    public:
        IGfxResource():INamed(){}

        template<class _GfxResource>
        static SharedPtr<_GfxResource> Create(const char * name = "unknown")
        {
            auto resource = TS_NEW(_GfxResource)();
            resource->SetIName(name);

            return SharedPtr<_GfxResource>(resource);
        }
    };

}
