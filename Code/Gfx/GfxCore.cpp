#include "pch.h"
#include "GfxCore.h"

namespace TS
{
    GfxCore::GfxCore(HWND windowHandle)
    {
        m_pDevice = IGfxResource::Create<GfxDevice>("GfxDevice");
        m_pDevice->Initialize(windowHandle);
    }

    void GfxCore::BeginScene()
    {

    }

    void GfxCore::ClearColor(float r, float g, float b, float a)
    {
        float c[4]{ r,g,b,a };
        m_pDevice->GetImmediateContext()->ClearColor(c);
    }

    void GfxCore::EndScene()
    {
        m_pDevice->GetDxSwapChain()->Present(0, 0);
    }
}
