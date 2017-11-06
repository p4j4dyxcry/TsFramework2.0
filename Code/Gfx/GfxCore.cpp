#include "pch.h"
#include "GfxCore.h"

TS::GfxCore::GfxCore(HWND windowHandle)
{
    m_pDevice = IGfxResource::Create<GfxDevice>("GfxDevice");
    m_pDevice->Initialize(windowHandle);
}
