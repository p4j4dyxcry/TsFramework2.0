#include "GfxResourceService.h"

namespace TS
{
    static GfxResourceService * g_instance = nullptr;
    GfxResourceService* GfxResourceService::Instance()
    {
        if (g_instance == nullptr)
            g_instance = new GfxResourceService();
        return g_instance;
    }

    std::list<WeakPtr<IGfxResource>> GfxResourceService::Find(const char* str)
    {
        return m_finder.Find([&](WeakPtr<IGfxResource> resource)
        {
            return BMSearch(resource->GetIName(), str) != -1;
        });
    }

    void GfxResourceService::Register(const char* name, WeakPtr<IGfxResource> resource)
    {
        std::string str = name;
        for (int index = 1; m_finder.Exists(str); ++index)
        {
            std::ostringstream stream;
            stream << name << "(" << index << ")";
            str = stream.str();
        }
        resource->SetIName(str.c_str());
        m_finder.Register(str, resource);
    }
}
