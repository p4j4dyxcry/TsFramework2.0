#pragma once
#include "Code/Core/Object.h"
#include "Code/Utility/Finder.h"

namespace TS
{
    class GfxResourceService : public Object
    {
    private:
        Finder<std::string, WeakPtr<IGfxResource>> m_finder;
        TS_DISABLE_COPY(GfxResourceService);
        TS_DISABLE_MOVE(GfxResourceService);
    public:        
        static GfxResourceService* Instance();

        template<class _GfxResource>
        SharedPtr<_GfxResource> Create(const char * name = "unknown")
        {
            auto resource = SharedPtr<_GfxResource>(TS_NEW(_GfxResource)());
            Register(name, resource);
            return resource;
        }

        std::list<WeakPtr<IGfxResource>> Find(const char* str);
    private:
        GfxResourceService() {}
        void Register(const char* name, WeakPtr<IGfxResource> resource);
    };
}

