#pragma once
#include "GfxDeviceContext.h"

namespace TS
{
    class GfxDevice :public IGfxResource
    {
    private:
        SharedPtr<GfxDeviceContext>     m_pDxImmediateContext;
        SharedPtr<IDXGISwapChain>       m_pDxSwapChain;
        SharedPtr<ID3D11Device>         m_pDx11Device;
    public:
        GfxDevice():IGfxResource()
        {}
        bool Initialize(HWND windowHandle);
        WeakPtr<ID3D11Device> GetDxDevice() const;
        WeakPtr<IDXGISwapChain> GetDxSwapChain() const;

        SharedPtr<ID3D11DeviceContext> CreateDefferdContext();
    };

}
