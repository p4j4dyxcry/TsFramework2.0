#include "Window.h"

namespace TS
{
    Window::Window(): m_WindowHandle(nullptr)
                      , m_ParentWindowHandle(nullptr)
                      , m_Width(0)
                      , m_Height(0), m_isRuning(false)
    {
    }

    bool Window::Create(const char* _className, const char* _windowTitle, const int width, const int height,
                        const HWND owner)
    {
        TS_LOCK( Mutex());

        if (m_WindowHandle != nullptr)
            return false;

        // 定義作成
        WNDCLASSEX wc =
        {
            sizeof(WNDCLASSEX),
            CS_CLASSDC,                         //! ウィンドウスタイル
            Window::CallWindowProcedure,        //! プロシージャ呼び出し
            0,                                  //! クラス拡張
            0,                                  //! ウィンドウ拡張
            GetModuleHandle(nullptr),           //プロセスID
            nullptr,                            //! アイコン
            LoadCursor(nullptr, IDC_ARROW),     //! カーソル
            nullptr,                            //! 背面色
            nullptr,                            //! メニュー
            _className,                         //! ユニークな名前
            nullptr                             //! サムネイル
        };

        //! 登録要求
        if (RegisterClassEx(&wc) == 0)
        {
            TS_LOG("指定された識別子は既に登録済みです。\n%S\n", _className);
            return false;
        }

        //! インスタンス化
        const HWND hwnd = CreateWindow(
            _className,                         //! ユニークな名前
            _windowTitle,                       //! タイトル
            WS_OVERLAPPEDWINDOW,                //! スタイル
            CW_USEDEFAULT,                      //! X
            CW_USEDEFAULT,                      //! Y
            width,                              //! 幅
            height,                             //! 高さ
            owner,                              //! 親ウィンドウへのハンドル
            nullptr,                            //! メニューへのハンドル
            wc.hInstance,                       //! プロセスID
            this);                              //! カスタム領域

        //! プロパティにコピー
        strcpy_s(m_ClassName, 256, _className);
        m_Width = width;
        m_Height = height;
        m_WindowHandle = hwnd;
        m_ParentWindowHandle = owner;

        return true;
    }

    HWND Window::GetHandle() const
    {
        return m_WindowHandle;
    }

    void Window::Run()
    {
        if (m_WindowHandle == nullptr)
            return;

        m_isRuning = true;
        ShowWindow(m_WindowHandle, SW_SHOWDEFAULT);
        UpdateWindow(m_WindowHandle);
    }

    void Window::Close()
    {
        if (m_WindowHandle == nullptr)
            return;

        m_isRuning = false;
        DestroyWindow(m_WindowHandle);
        m_WindowHandle = nullptr;
    }

    bool Window::IsRuning() const
    {
        return m_isRuning;
    }

    LRESULT Window::WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        if (msg == WM_CLOSE || msg == WM_DESTROY)
        {
            Close();
            return 0;
        }

        return DefWindowProc(hWnd, msg, wp, lp);
    }

    Window* Window::GetParent() const
    {
        return GetWindow(m_ParentWindowHandle);
    }

    Window* Window::GetWindow(HWND hWnd)
    {
        return reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    void Window::ProsessMessage()
    {
        MSG msg;
        //! 直にGetMessageを呼び出すと操作していないときに止まるので注意。
        if(PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE))
        {
            if (GetMessage(&msg, nullptr, 0, 0) != -1)
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    void Window::GetClientSize(int& _outWidth, int& _outHeight) const
    {
        RECT rect;
        GetClientRect(m_WindowHandle,&rect);
        _outWidth = rect.right;
        _outHeight = rect.bottom;
    }

    LRESULT Window::CallWindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        Window* window = GetWindow(hWnd);

        //! プロシージャを呼び出す
        if (window != nullptr)
            return window->WindowProc(hWnd, msg, wp, lp);

        //! ここに来るということは関連付けがまだできていない
        if (msg == WM_CREATE)
        {
            //! カスタム領域からWindowクラスを持ってくる
            window = reinterpret_cast<Window*>(reinterpret_cast<LPCREATESTRUCT>(lp)->lpCreateParams);
        }
        if (window != nullptr)
        {
            //! 関連付け
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        }

        return DefWindowProc(hWnd, msg, wp, lp);
    }
}
