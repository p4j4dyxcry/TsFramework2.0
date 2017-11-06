#include "pch.h"
#include "GfxRenderTarget.h"

TS::GfxRenderTarget::GfxRenderTarget(): GfxTexture2D(), m_width(0), m_height(0)
{
}

bool TS::GfxRenderTarget::Initialize(WeakPtr<ID3D11Device> pDevice, 
                                     const int width,
                                     const int height,
                                     const DXGI_FORMAT format)
{
    //! 2Dテクスチャの作成
    {
         D3D11_TEXTURE2D_DESC texDesc;
         texDesc.ArraySize = 1;
         texDesc.CPUAccessFlags = 0;
         texDesc.MipLevels = 1;
         texDesc.MiscFlags = 0;
         texDesc.SampleDesc.Count = 1;
         texDesc.SampleDesc.Quality = 0;
         texDesc.Usage = D3D11_USAGE_DEFAULT;

         texDesc.Width = width;
         texDesc.Height = height;
         texDesc.Format = format;

         texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

        ID3D11Texture2D * pTex2D;
        HRESULT hr = pDevice->CreateTexture2D(&texDesc, nullptr, &pTex2D);

        if(FAILED(hr))
        {
            TS_LOG_ERROR("レンダーターゲット作成に失敗\n");
            return false;
        }

        m_pDxTexture2D = CreateDxSharePtr(pTex2D);
    }

    //! レンダーターゲットビューの作成
    {
        ID3D11RenderTargetView * pRTV;
        HRESULT hr = pDevice->CreateRenderTargetView( m_pDxTexture2D.GetPointer(), nullptr, &pRTV);
        m_pDxRenderTargetView = CreateDxSharePtr(pRTV);

        if (FAILED(hr))
        {
            TS_LOG_ERROR("レンダーターゲット作成に失敗\n");
            return false;
        }
    }
    //! シェーダリソースビューの作成
    {
        ID3D11ShaderResourceView * pSRV;
        HRESULT hr = pDevice->CreateShaderResourceView(m_pDxTexture2D.GetPointer(), nullptr, &pSRV);
        m_pDxShaderResourceView = CreateDxSharePtr(pSRV);

         if (FAILED(hr))
        {
            TS_LOG_ERROR("レンダーターゲット作成に失敗\n");
            return false;
        }
    }

    m_width = width;
    m_height = height;

    return true;
}

bool TS::GfxRenderTarget::Initialize( WeakPtr<ID3D11Device>   pDevice,
                                      WeakPtr<IDXGISwapChain> pSwapChain)
{

    // Swap Chain から　バックバッファへのポインタを取得
    ID3D11Texture2D* pBackBuffer = nullptr;
    ID3D11RenderTargetView* pD3Drtv;
    HRESULT hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));
    auto ptr = CreateDxSharePtr(pBackBuffer);

    if (FAILED(hr))
    {
        TS_LOG_ERROR("レンダーターゲット作成に失敗\n");
        return false;
    }

    // バックバッファへのポインタを指定してRTVを作成
    hr = pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pD3Drtv);
    D3D11_TEXTURE2D_DESC desc;
    pBackBuffer->GetDesc(&desc);
    m_pDxRenderTargetView = CreateDxSharePtr(pD3Drtv);
    m_width  = desc.Width;
    m_height = desc.Height;

    if (FAILED(hr))
    {
        TS_LOG_ERROR("レンダーターゲット作成に失敗\n");
        return false;
    }
    return true;
}
