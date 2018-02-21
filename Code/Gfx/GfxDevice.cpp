#include "pch.h"
#include "GfxDevice.h"
#include "Code/Utility/Window.h"
#include "GfxResourceService.h"

namespace TS
{
    TS::GfxDevice::GfxDevice() : IGfxResource()
    {
    }

    bool TS::GfxDevice::Initialize(HWND windowHandle)
    {
        //! スワップチェイン
        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        swapChainDesc.BufferCount = 1;
        swapChainDesc.Windowed = true;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        swapChainDesc.OutputWindow = windowHandle;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

        const auto window = Window::GetWindow(windowHandle);
        int clientWidth, clientHeight;
        window->GetClientSize(clientWidth, clientHeight);

        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.Width = static_cast<unsigned>(clientWidth);
        swapChainDesc.BufferDesc.Height = static_cast<unsigned>(clientHeight);
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

        swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 60;

        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;

        D3D_DRIVER_TYPE driverTypes[] =
        {
            D3D_DRIVER_TYPE_HARDWARE ,
        };
        unsigned numDriverTypes = ARRAYSIZE(driverTypes);

        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_1 ,
            D3D_FEATURE_LEVEL_11_0 ,
        };

        unsigned numFeatureLevels = ARRAYSIZE(featureLevels);

        unsigned devCreateFlag = 0;
        devCreateFlag |= D3D11_CREATE_DEVICE_DEBUG;

        HRESULT hr;

        ID3D11Device*           pDevice;
        IDXGISwapChain*         pSwapChain;
        ID3D11DeviceContext*    pDeviceContext;

        for (unsigned i = 0; i < numDriverTypes; ++i)
        {
            D3D_FEATURE_LEVEL featureLeve;

            hr = D3D11CreateDeviceAndSwapChain(
                nullptr,           // ディスプレイデバイスのアダプタ（ＮＵＬＬの場合最初に見つかったアダプタ）
                driverTypes[i],    // デバイスドライバのタイプ
                nullptr,           // ソフトウェアラスタライザを使用する場合に指定する
                devCreateFlag,     // デバイスの生成フラグ
                featureLevels,     // 機能レベル
                numFeatureLevels,  // 機能レベル数
                D3D11_SDK_VERSION, // Dx11 バージョン
                &swapChainDesc,    // スワップチェインの設定
                &pSwapChain,       // IDXGIDwapChainインタフェース	
                &pDevice,          // ID3D11Deviceインタフェース
                &featureLeve,      // サポートされている機能レベル
                &pDeviceContext);  // デバイスコンテキスト

            if (SUCCEEDED(hr))
                break;
        }
        if (FAILED(hr))
        {
            TS_LOG_ERROR("DirectX11のデバイス生成に失敗。\n");
            return false;
        }
        m_pDx11Device = CreateDxSharePtr(pDevice);
        m_pDxSwapChain = CreateDxSharePtr(pSwapChain);

        m_pDxImmediateContext = GfxResourceService::Instance()->Create<GfxDeviceContext>();
        m_pDxImmediateContext->Initialize(CreateDxSharePtr(pDeviceContext), m_pDx11Device, m_pDxSwapChain);

        return true;
    }

    TS::SharedPtr<TS::GfxDeviceContext>& TS::GfxDevice::GetImmediateContext()
    {
        return m_pDxImmediateContext;
    }

    TS::SharedPtr<struct ID3D11DeviceContext> TS::GfxDevice::CreateDefferdContext()
    {
        ID3D11DeviceContext* pDefferdContext;
        m_pDx11Device->CreateDeferredContext(0, &pDefferdContext);
        return CreateDxSharePtr(pDefferdContext);
    }

    TS::WeakPtr<struct IDXGISwapChain> TS::GfxDevice::GetDxSwapChain() const
    {
        return WeakPtr<IDXGISwapChain>(m_pDxSwapChain);
    }

    TS::WeakPtr<struct ID3D11Device> TS::GfxDevice::GetDxDevice() const
    {
        return WeakPtr<ID3D11Device>(m_pDx11Device);
    }

}

