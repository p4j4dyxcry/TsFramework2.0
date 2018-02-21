#pragma once
#include "Thread.h"
#include "Code/Utility/Window.h"
#include "TimeSystem.h"


namespace TS
{
    /**
	 * \brief エンジン起動オプション
	 */
	struct EngineSetting 
	{
    public:
        struct WindowSetting
        {
            std::string Title;
            int  Width;
            int  Height;

            template <class Archive>
            void serialize(Archive & archive)
            {
                archive(CEREAL_NVP(Title), CEREAL_NVP(Width),CEREAL_NVP(Height));
            }
           
            WindowSetting();
        };

        struct TimeScaleSetting
        {
            int  Framerate;

            TimeScaleSetting();
            template <class Archive>
            void serialize(Archive & archive)
            {
                archive(CEREAL_NVP(Framerate));
            }
        };
        WindowSetting Window;
        TimeScaleSetting Time;
	    EngineSetting();

        template <class Archive>
        void serialize(Archive & archive)
        {
            archive(CEREAL_NVP(Window),
                CEREAL_NVP(Time));
        }
	};

    /**
	 * \brief TsFrameworkのコアシステム
	 */
	class Engine : public IMutex
	{
	private:
		TS_DISABLE_COPY(Engine);
		TS_DISABLE_MOVE(Engine);
	    Engine();

        bool Initialize(EngineSetting& option);

        void Finalize();

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
         * \brief エンジンを更新する
         */
        void UpdateEngine();


	    /**
		 * \brief エンジン稼働中かどうか
		 * \return 稼働中ならtrue
		 */
		bool IsRuning()const;


        /**
        * \brief 独自で定義したロガーを使用する場合はこの関数で設定する
        * \param logger
        */
        void SetLogger(const SharedPtr<Logger>& logger);

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

	    /**
         * \brief タイムシステムを取得する
         * \return 
         */
        SharedPtr<TimeSystem>& GetTimeSystem();

	    /**
         * \brief フレーム終了処理
         */
        void EndFrame();

        /**
        * \brief グラフィックスシステムを取得する
        * \return
        */
        SharedPtr<GfxCore>& GetGfxSystem();
	    const SharedPtr<GfxCore>& GetGfxSystem()const;
	   
	 private:
		MemorySystem*	        m_pMemorySystem;
		SharedPtr<Logger>       m_pUserLogger;
		SharedPtr<Window>       m_pMainWindow;
        SharedPtr<GfxCore>      m_pGfxCore;
        SharedPtr<TimeSystem>   m_pTimeSystem;
	};

    /**
	 * \brief エンジンを取得　この関数は Engine.Instance()と等しい
	 * \return 
	 */
	Engine* GetEngine();
}
