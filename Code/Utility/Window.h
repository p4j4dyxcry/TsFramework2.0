#pragma once

#include <Windows.h>

namespace TS
{
    /**
	 * \brief ウィンドウクラス
	 */
	class Window : IMutex
	{
	public:
	    /**
		 * \brief コンストラクタ
		 */
	    Window();

	    /**
		 * \brief windowを作成する
		 * \param _className    一意なウィンドウの内部名
		 * \param _windowTitle  ウィンドウタイトル
		 * \param width         幅
		 * \param height        高さ
		 * \param owner         親ウィンドウのハンドル
		 * \return 成功ならtrue
		 */
	    bool Create(const char* _className,
	                const char* _windowTitle,
	                const int width = 1280,
	                const int height = 720,
	                const HWND owner = nullptr);

	    /**
		 * \brief ウィンドウハンドルを取得する
		 * \return 
		 */
	    HWND GetHandle() const;

	    /**
	     * \brief ウィンドウを作動させる
	     */
	    void Run();

	    /**
	     * \brief ウィンドウを閉じる
	     */
	    void Close();

	    /**
		 * \brief ウィンドウが作動しているかどうか
		 * \return 
		 */
	    bool IsRuning() const;

	    /**
		 * \brief ウィンドウプロシージャ
		 * \param hWnd 
		 * \param msg 
		 * \param wp 
		 * \param lp 
		 * \return 
		 */
	    virtual LRESULT WINAPI WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
	    /**
		 * \brief 親ウィンドウの取得
		 * \return 
		 */
	    Window* GetParent() const;

	    /**
		 * \brief 指定されたハンドルを持つWindowクラスを取得する
		 * \param hWnd 
		 * \return 存在しなければ null
		 */
	    static Window* GetWindow(HWND hWnd);

	private:
	    /**
		 * \brief ウィンドウプロシージャを呼び出す
		 * \param hWnd 
		 * \param msg 
		 * \param wp 
		 * \param lp 
		 * \return 
		 */
	    static LRESULT WINAPI CallWindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
		HWND m_WindowHandle;
		HWND m_ParentWindowHandle;
		char m_ClassName[256];
		int  m_Width;
		int  m_Height;
		bool m_isRuning;
	};
}
