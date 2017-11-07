#pragma once
#include "Code/Utility/StopWatch.h"

namespace TS
{
    /**
     * \brief 時間管理システム
     */
    class TimeSystem
    {
    public:
        /**
         * \brief コンストラクタ
         * \param frameRate 
         */
        TimeSystem(int frameRate);

        /**
         * \brief フレームレートを設定する
         * \param framerate 
         */
        void SetFramerate(int framerate);

        /**
         * \brief フレームレートの取得
         * \return 
         */
        int  GetFrameRate()const;

        /**
         * \brief メインループの初めに呼ぶ
         */
        void BeginFrame();

        /**
         * \brief メインループの終わりに呼ぶ
         */
        void EndFrame();

        /**
         * \brief 余った処理時間を待機する
         */
        void WaitRemaining();

        /**
         * \brief 前フレームの経過時間を取得する
         * \return 
         */
        double GetDeltaTime() const;

        /**
         * \brief FPSを一定間隔で出力する
         * \param isEnable trueなら有効
         * \param interval 出力感覚(秒)
         */
        void EnableDumpFps(bool isEnable, double interval = 5);
    private:
        TimeSystem();
        void DumpFps(double intervalSec);

    private:
        StopWatch         m_StopWatch;
        int               m_Framerate;
        double            m_dumpFpsInterval;
    };
}

