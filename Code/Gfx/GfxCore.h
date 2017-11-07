#pragma once
#include "GfxDevice.h"

namespace TS
{
    class GfxCore :public  Object
    {
    private:
        SharedPtr<GfxDevice> m_pDevice;
    public:
        /**
         * \brief コンストラクタ
         * \param windowHandle 
         */
        GfxCore( HWND windowHandle );
        /**
         * \brief シーン開始
         */
        void BeginScene();

        void ClearColor(float r, float g, float b, float a);

        /**
         * \brief シーン終了
         */
        void EndScene();
    };

}
