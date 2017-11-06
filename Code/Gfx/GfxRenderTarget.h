#pragma once
#include "IGfxTexture.h"

namespace TS
{
    class GfxRenderTarget : public GfxTexture2D
    {
    public:
        GfxRenderTarget();


        /**
         * \brief サイズとフォーマットからレンダーターゲットビューを初期化
         * \param pDevice 
         * \param width 
         * \param height 
         * \param format 
         * \return 成功なら true
         */
        bool Initialize(WeakPtr<ID3D11Device> pDevice,
                        const int width,
                        const int height,
                        DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

        /**
         * \brief スワップチェインからレンダーターゲットを初期化
         * \param pDevice      
         * \param pSwapChain   
         * \return 成功なら true
         */
        bool Initialize(WeakPtr<ID3D11Device> pDevice,
                        WeakPtr<IDXGISwapChain>  pSwapChain);

        SharedPtr<ID3D11RenderTargetView> GetRenderTargetView()const { return m_pDxRenderTargetView; }
    private:
        SharedPtr<ID3D11RenderTargetView> m_pDxRenderTargetView;
        int m_width;
        int m_height;
    };
}
