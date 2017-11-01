#pragma once

namespace TS
{
    /**
     * \brief ログのレベルを定義します
     */
    enum LogLevel 
	{
        Log_Debug,                 //! デバッグ用
        Log_Info,                  //! 想定通りの事象
        Log_Warning,               //! 想定外の事象
        Log_Error,                 //! 致命的な問題
    };                                 
   
    /**
     * \brief ログを出力するためのメタデータを定義します
     */
    struct LogMetaData
    {
        int line;                   //! ソースコード内での行番号
        const char* fileName;       //! ソースコードのファイル名
        const char* functionName;   //! 関数名
        LogLevel logLevel;          //! 重大性
    };

    
    /**
     * \brief ログ出力クラスを定義します。
     * 　　　 ユーザが任意のログを利用したい場合はこのクラスを継承して実装します。
     */
    class Logger 
    {
    public:
        virtual ~Logger() = default;
		virtual const char* Format(const char * format, ...);
        virtual void PreLog(LogMetaData& metaData) {};
        virtual void Log(LogMetaData& metaData , const char * format);
        virtual void EndLog(LogMetaData& metaData) {};
    };

    /**
     * \brief ユーザ定義のロガーを設定します。
     * \param logger ユーザが作成したロガークラスです。
     */
    void SetUserLogger(const SharedPtr<Logger>& logger);
    
    /**
    * \brief 現在適用されているロガーを取得します。デフォルトのロガーはprintf関数に近い動作をします。
    * \return 設定されていないときはデフォルトのロガーが返却されます
    */
    Logger& GetLogger();
}
