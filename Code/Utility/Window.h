#pragma once

#include <Windows.h>

namespace TS
{
	class Window : IMutex
	{
	public:
		Window():m_WindowHandle(nullptr)
		{
		}

		bool Create( const char* _className,
					 const char* _windowTitle,
					 const int width  = 1280,
					 const int height = 720,
					 const HWND owner = nullptr)
		{
			TS_LOCK( Mutex());

			if (m_WindowHandle != nullptr)
				return false;

			// Create application window
			WNDCLASSEX wc =
			{
				sizeof(WNDCLASSEX),					
				CS_CLASSDC,
				Window::CallWindowProcedure,		//プロシージャ
				0,
				0,
				GetModuleHandle(nullptr),			//プロセスID
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
			HWND hwnd = CreateWindow(	_className, 
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

		HWND GetHandle()const
		{
			return m_WindowHandle;
		}

		void Run()
		{
			TS_LOCK(Mutex());

			if (m_WindowHandle == nullptr)
				return;

			m_isRuning = true;
			ShowWindow(m_WindowHandle, SW_SHOWDEFAULT);
			UpdateWindow(m_WindowHandle);
		}

		void Close()
		{
			//TS_LOCK(Mutex());
		
			if (m_WindowHandle == 0)
				return;

			m_isRuning = false;
			DestroyWindow(m_WindowHandle);
			m_WindowHandle = nullptr;
		}

		bool IsRuning()const
		{
			return m_isRuning;
		}

		virtual LRESULT WINAPI WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			if (msg == WM_CLOSE || msg == WM_DESTROY)
			{				
				Close();
				return 0;
			}

			return DefWindowProc(hWnd, msg, wp, lp);
		}
		Window* GetParent()
		{
			return GetWindow(m_ParentWindowHandle);
		}

		static Window* GetWindow(HWND hWnd)
		{
			return (Window*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		}

	private:
		static LRESULT WINAPI CallWindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			Window* window = Window::GetWindow(hWnd);

			if (window != nullptr)
				return window->WindowProc(hWnd, msg, wp, lp);
			else
			{
				if (msg == WM_CREATE)
				{
					window = reinterpret_cast<Window*>(((LPCREATESTRUCT)lp)->lpCreateParams);
				}
				if (window != nullptr)
				{
					SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
				}
			}

			return DefWindowProc(hWnd, msg, wp, lp);;
		}
		HWND m_WindowHandle;
		HWND m_ParentWindowHandle;
		char m_ClassName[256];
		int  m_Width;
		int  m_Height;
		bool m_isRuning;
	};
}