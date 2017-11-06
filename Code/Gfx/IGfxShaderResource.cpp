#include "pch.h"
#include "IGfxShaderResource.h"

namespace TS
{
    IGfxShaderResource::IGfxShaderResource() : IGfxResource(), m_registerID(0)
    {

    }

    void IGfxShaderResource::SetRegisterID(const int id)
    {
        m_registerID = id;
    }

    int IGfxShaderResource::GetRegisterID() const
    {
        return m_registerID;
    }
}
