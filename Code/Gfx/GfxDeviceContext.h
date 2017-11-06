#pragma once
#include "GfxRenderTarget.h"

namespace TS
{
    class GfxDeviceContext : public IGfxResource
    {
    public:
        SharedPtr<ID3D11DeviceContext> m_pDeviceContext;
        SharedPtr<GfxRenderTarget>     m_pMainRenderTarget;

        bool Initialize(SharedPtr<ID3D11DeviceContext> pDeviceContext,
                        WeakPtr<ID3D11Device> pDevice,
                        WeakPtr<IDXGISwapChain> pSwapChain)
        {
            m_pDeviceContext = pDeviceContext;
            m_pMainRenderTarget = IGfxResource::Create<GfxRenderTarget>("MainRenderTarget");
            m_pMainRenderTarget->Initialize(pDevice, pSwapChain);

            float c[4] = { 0,0,0,1 };
            m_pDeviceContext->ClearRenderTargetView(m_pMainRenderTarget->GetRenderTargetView().GetPointer(), c);

            pSwapChain->Present(0, 0);

            return true;
        }
    };

}
