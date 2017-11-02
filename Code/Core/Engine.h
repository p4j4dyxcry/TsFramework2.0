#pragma once
#include "Thread.h"
#include "Code/Utility/Window.h"

namespace TS
{
	struct EngineSetting 
	{
	public:
		std::string windowTitle;
		int  windowWidth;
		int  windowHeight;
		EngineSetting()
			:windowTitle("TsFramework")
			,windowWidth(1280)
			,windowHeight(768)
		{};

	};

    /**
	 * \brief TsFrameworkのコアシステム
	 */
	class Engine : public IMutex
	{
	private:
		TS_DISABLE_COPY(Engine);
		TS_DISABLE_MOVE(Engine);
	    Engine();;
	public:

	    /**
        * \brief デストラクタ
        */
        virtual ~Engine();


	    /**
	     * \brief インスタンスを取得する
	     * \return 
	     */
	    static Engine* Instance();

		/**
		* \brief エンジンの破棄
		*/
		static Engine* Create(EngineSetting option = EngineSetting());

	    /**
	     * \brief エンジンの破棄
	     */
	    static void Destory();

	    /**
	     * \brief 独自で定義したロガーを使用する場合はこの関数で設定する
	     * \param logger 
	     */
	    void SetLogger(const SharedPtr<Logger>& logger);

		bool IsRuning()const;

		void UpdateEngine();

	    /**
	     * \brief ロガーを取得する。何も設定していない場合はデフォルトを取得する
	     * \return 
	     */
	    SharedPtr<Logger> GetLogger() const;

	    /**
	     * \brief メモリシステムを取得する
	     * \return 
	     */
	    MemorySystem&     GetMemorySystem() const;
	private:
		MemorySystem*	  m_pMemorySystem;
		SharedPtr<Logger> m_pUserLogger;
		SharedPtr<Window> m_MainWindow;
	};

    /**
	 * \brief エンジンを取得　この関数は Engine.Instance()と等しい
	 * \return 
	 */
	Engine* GetEngine();
}
