#pragma once
#include "GfxDevice.h"

namespace TS
{
    class GfxCore :public  Object
    {
    private:
        SharedPtr<GfxDevice> m_pDevice;
    public:
        GfxCore( HWND windowHandle );

    };

}
