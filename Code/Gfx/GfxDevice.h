#pragma once
#include "GfxDeviceContext.h"

namespace TS
{
    class GfxDevice :public IGfxResource
    {
    public:
        /**
         * \brief コンストラクタ
         */
        GfxDevice();

        /**
         * \brief 初期化
         * \param windowHandle 
         * \return 
         */
        bool Initialize(HWND windowHandle);
        /**
         * \brief ID3D11Deviceを取得する
         * \return 
         */
        WeakPtr<ID3D11Device> GetDxDevice() const;
        /**
         * \brief IDXGISwapChainを取得する
         * \return 
         */
        WeakPtr<IDXGISwapChain> GetDxSwapChain() const;

        /**
         * \brief デバイスコンテキストを主とする
         * \return 
         */
        SharedPtr<GfxDeviceContext>& GetImmediateContext();

        /**
         * \brief 遅延コンテキストを作成する
         * \return 遅延コンテキスト
         */
        SharedPtr<ID3D11DeviceContext> CreateDefferdContext();

    private:
        SharedPtr<GfxDeviceContext>     m_pDxImmediateContext;
        SharedPtr<IDXGISwapChain>       m_pDxSwapChain;
        SharedPtr<ID3D11Device>         m_pDx11Device;
    };

}
