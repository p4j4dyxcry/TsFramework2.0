#pragma once

/**
* \brief 開発支援機能
*/

#include "Logger.h"

//!  コピーコンストラクタとコピー代入演算子を禁止
#define TS_DISABLE_COPY( type ) \
    type( const type& ) = delete; \
    type& operator=( const type& ) = delete

//!  ムーブコンストラクタとムーブ代入演算子を禁止
#define TS_DISABLE_MOVE( type ) \
    type( type&& ) = delete; \
    type& operator=( type&& ) = delete

//!  意図した未使用変数に対するコンパイラ警告を抑制
#define TS_UNUSED( variable ) \
    ( void )( variable )

//! 対象の利用は非推奨であることを宣言
#define TS_DEPRECATED __declspec(deprecated("非推奨な項目を利用しています。この機能は廃止予定です。"))
#define TS_DEPRECATED_MESSAGE (message) __declspec(deprecated(message))

//! UINT最大値をfloat表現
#define TS_UINT_MAX_TO_FLT ( 4294967295.0f )

#define TS_UINT_MAX_TO_DBL ( 4294967295.0  )

//!  現在の関数の名前
#define TS_FUNCTION __FUNCTION__

//! 現在のファイルの名前
#define TS_FILENAME __FILE__

//! 現在の行数
#define TS_LINE __LINE__

//!  関数が例外を投げないことを宣言
#define TS_NOEXCEPT throw()

//! ログを出力
#define TS_LOG(...)\
    {\
        TS::LogMetaData metadata; \
        metadata.line = TS_LINE; \
        metadata.fileName = TS_FILENAME; \
        metadata.functionName = TS_FUNCTION; \
        metadata.logLevel = TS::Log_Info; \
        TS::GetLogger().Log(metadata, TS::GetLogger().Format(__VA_ARGS__));\
    }

//! デバッグログを出力
#ifdef _DEBUG
    #define TS_LOG_DEBUG(...)\
        {\
            TS::LogMetaData metadata; \
            metadata.line = TS_LINE; \
            metadata.fileName = TS_FILENAME; \
            metadata.functionName = TS_FUNCTION; \
            metadata.logLevel = TS::Log_Debug; \
            TS::GetLogger().Log(metadata, TS::GetLogger().Format(__VA_ARGS__));\
        }
#else
    #define TS_LOG_DEBUG(...);
#endif


//! ワーニングログ
#define TS_LOG_WARNING(...)\
    {\
        TS::LogMetaData metadata; \
        metadata.line = TS_LINE; \
        metadata.fileName = TS_FILENAME; \
        metadata.functionName = TS_FUNCTION; \
        metadata.logLevel = TS::Log_Warning; \
        TS::GetLogger().Log(metadata, TS::GetLogger().Format(__VA_ARGS__));\
    }

//! エラーログを出力
#define TS_LOG_ERROR(...)\
    {\
        TS::LogMetaData metadata; \
        metadata.line = TS_LINE; \
        metadata.fileName = TS_FILENAME; \
        metadata.functionName = TS_FUNCTION; \
        metadata.logLevel = TS::Log_Error; \
        TS::GetLogger().Log(metadata, TS::GetLogger().Format(__VA_ARGS__));\
    }

#define TS_ASSERT(flag , ...)\
{\
    if(flag)\
    {\
        TS_LOG_ERROR(__VA_ARGS__);\
        MessageBox(nullptr, TS::GetLogger().Format(__VA_ARGS__),"致命的なエラー",MB_OK);\
        \
    }\
}

//! 現在時刻を出力する
#define TS_DUMP_CURRENT_TIME(action) \
{\
    time_t t = time(nullptr);\
    \
    struct tm lt;\
    localtime_s(&lt, &t);\
    \
    std::stringstream s;\
    s << action << " ::";\
    s << "20" << lt.tm_year - 100 << "/" << lt.tm_mon + 1 << "/" << lt.tm_mday << " ";\
    s << lt.tm_hour << "時" << lt.tm_min << "分" << lt.tm_sec << "秒";\
    \
    TS_LOG("//!---------------------------------------------\n");\
    TS_LOG("//! %s\n", s.str().c_str());\
    TS_LOG("//!---------------------------------------------\n");\
}