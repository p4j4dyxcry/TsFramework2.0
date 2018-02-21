#pragma once
#include "GfxRenderTarget.h"

namespace TS
{
    class GfxDeviceContext : public IGfxResource
    {
    private:
        static const int NumRenderTargetSlot = 8;

    public:
        /**
         * \brief コンストラクタ
         */
        GfxDeviceContext();
        /**
         * \brief レンダーターゲットを設定する
         * \param renderTarget 設定するレンダーターゲット
         * \param id スロット番号
         */
        void SetRenderTarget(SharedPtr<GfxRenderTarget>& renderTarget ,  int id = 0);

        /**
         * \brief レンダーターゲットの設定を解除する
         * \param id スロット番号
         */
        void UnSetRenderTarget( int id = 0);

        /**
         * \brief 指定色で対象のレンダーターゲットを塗りつぶす
         * \param col 
         * \param renderTarget 
         */
        void FillRenderTarget(const float col[4], SharedPtr<GfxRenderTarget> renderTarget = nullptr);

        /**
         * \brief 全てのレンダーターゲットスロットを指定色で塗りつぶす
         * \param col 
         */
        void FillRenderTargetsAll(const float col[4]);

        /**
         * \brief 初期化
         * \param pDeviceContext 
         * \param pDevice 
         * \param pSwapChain 
         * \return 
         */
        bool Initialize(SharedPtr<ID3D11DeviceContext> pDeviceContext,
                        WeakPtr<ID3D11Device> pDevice,
                        WeakPtr<IDXGISwapChain> pSwapChain);

        /**
         * \brief トポロジを設定する
         * \param topology 
         */
        void SetTopology(Topology topology);
    private:
        SharedPtr<ID3D11DeviceContext> m_pDeviceContext;
        SharedPtr<GfxRenderTarget>     m_pMainRenderTarget;
        SharedPtr<GfxRenderTarget>     m_renderTargets[NumRenderTargetSlot];
        Topology                       m_topology;

    };

}
