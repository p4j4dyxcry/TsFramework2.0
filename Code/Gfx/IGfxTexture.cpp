#include "pch.h"
#include "IGfxTexture.h"
namespace TS
{
    IGfxTexture::IGfxTexture() : IGfxShaderResource()
    {
    }

    bool IGfxTexture::UseAlpha() const
    {
        return m_enableAlpha;
    }

    void IGfxTexture::SetAlphaEnable(bool flag)
    {
        m_enableAlpha = flag;
    }

    GfxTexture2D::GfxTexture2D()
    {
    }

    SharedPtr<ID3D11ShaderResourceView> IGfxTexture::GetShaderResourceView() const
    {
        return m_pDxShaderResourceView;
    }

}
