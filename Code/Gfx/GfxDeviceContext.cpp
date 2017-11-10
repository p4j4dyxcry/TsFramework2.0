#include "pch.h"
#include "GfxDeviceContext.h"

namespace TS
{
    GfxDeviceContext::GfxDeviceContext():m_topology(Topology::TriangleList)
    {

    }

    void GfxDeviceContext::SetRenderTarget(SharedPtr<GfxRenderTarget>& renderTarget, int id)
    {
        TS_ASSERT(id >= NumRenderTargetSlot && id < 0, "レンダーターゲットIDが不正\n");

        m_renderTargets[id] = renderTarget;
    }

    void GfxDeviceContext::UnSetRenderTarget(int id)
    {
        TS_ASSERT(id >= NumRenderTargetSlot && id < 0, "レンダーターゲットIDが不正\n");

        m_renderTargets[id] = nullptr;
    }

    void GfxDeviceContext::ClearColor(const float col[4], SharedPtr<GfxRenderTarget> renderTarget)
    {
        if (renderTarget != nullptr)
            m_pDeviceContext->ClearRenderTargetView(renderTarget->GetRenderTargetView().GetPointer(), col);
        else
            ClearColorByAllRenderTargetSlot(col);
    }

    void GfxDeviceContext::ClearColorByAllRenderTargetSlot(const float col[4])
    {
        for (int i = 0; i < NumRenderTargetSlot; ++i)
        {
            if (m_renderTargets[i] != nullptr)
            {
                m_pDeviceContext->ClearRenderTargetView(m_renderTargets[i]->GetRenderTargetView().GetPointer(), col);
            }
        }
    }

    bool GfxDeviceContext::Initialize(SharedPtr<ID3D11DeviceContext> pDeviceContext,
                                      WeakPtr<ID3D11Device> pDevice,
                                      WeakPtr<IDXGISwapChain> pSwapChain)
    {
        TS_ASSERT(pDeviceContext.IsNull() || pDevice.IsNull() || pSwapChain.IsNull(), "引き数が不正\n");

        m_pDeviceContext = pDeviceContext;
        m_pMainRenderTarget = Create<GfxRenderTarget>("MainRenderTarget");
        m_pMainRenderTarget->Initialize(pDevice, pSwapChain);
        m_renderTargets[0] = m_pMainRenderTarget;
        return true;
    }

    void GfxDeviceContext::SetTopology(const Topology topology)
    {
        m_topology = topology;
    }
}

