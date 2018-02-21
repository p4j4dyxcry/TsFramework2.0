#include "pch.h"
#include "GfxCore.h"
#include "GfxResourceService.h"

namespace TS
{
    GfxCore::GfxCore(HWND windowHandle)
    {
        m_pDevice = GfxResourceService::Instance()->Create<GfxDevice>("GfxDevice");
        m_pDevice->Initialize(windowHandle);
    }

    void GfxCore::BeginScene()
    {

    }

    void GfxCore::ClearColor(float r, float g, float b, float a)
    {
        float c[4]{ r,g,b,a };
        m_pDevice->GetImmediateContext()->FillRenderTarget(c);
    }

    void GfxCore::EndScene()
    {
        m_pDevice->GetDxSwapChain()->Present(0, 0);
    }
}
