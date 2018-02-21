#pragma once
#include "IGfxShaderResource.h"

namespace TS
{
    class IGfxTexture : public IGfxShaderResource
    {
    public:
        IGfxTexture();
        SharedPtr<ID3D11ShaderResourceView> GetShaderResourceView() const;
        bool UseAlpha()const;
        void SetAlphaEnable(bool flag);

    protected:
        SharedPtr<ID3D11ShaderResourceView> m_pDxShaderResourceView;
        bool m_enableAlpha;
    };

    class GfxTexture2D : public IGfxTexture
    {
    public:
        GfxTexture2D();
    protected:
        SharedPtr<ID3D11Texture2D> m_pDxTexture2D;
    };

}
