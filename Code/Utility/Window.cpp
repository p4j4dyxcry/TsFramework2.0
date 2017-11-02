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

        // Create application window
        WNDCLASSEX wc =
        {
            sizeof(WNDCLASSEX),
            CS_CLASSDC,
            Window::CallWindowProcedure, //プロシージャ
            0,
            0,
            GetModuleHandle(nullptr), //プロセスID
            nullptr,
            LoadCursor(nullptr, IDC_ARROW),
            nullptr,
            nullptr,
            _className,
            nullptr
        };

        if (RegisterClassEx(&wc) == 0)
        {
            TS_LOG("指定された識別子は既に登録済みです。\n%S\n", _className);
            return false;
        }
        HWND hwnd = CreateWindow( _className,
            _windowTitle,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            width,
            height,
            owner,
            nullptr,
            wc.hInstance,
            this);

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
        TS_LOCK(Mutex());

        if (m_WindowHandle == nullptr)
            return;

        m_isRuning = true;
        ShowWindow(m_WindowHandle, SW_SHOWDEFAULT);
        UpdateWindow(m_WindowHandle);
    }

    void Window::Close()
    {
        //TS_LOCK(Mutex());

        if (m_WindowHandle == 0)
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
        if (GetMessage(&msg, nullptr, 0, 0) != -1)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    LRESULT Window::CallWindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        Window* window = Window::GetWindow(hWnd);

        if (window != nullptr)
            return window->WindowProc(hWnd, msg, wp, lp);
        else
        {
            if (msg == WM_CREATE)
            {
                window = reinterpret_cast<Window*>(reinterpret_cast<LPCREATESTRUCT>(lp)->lpCreateParams);
            }
            if (window != nullptr)
            {
                SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
            }
        }

        return DefWindowProc(hWnd, msg, wp, lp);;
    }
}
