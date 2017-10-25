#pragma once

#include "Logger.h"

//!  コピーコンストラクタとコピー代入演算子を禁止
#define TS_DISABLE_COPY( type ) \
private: \
    type( const type& ); \
    type& operator=( const type& )

//!  ムーブコンストラクタとムーブ代入演算子を禁止
#define TS_DISABLE_MOVE( type ) \
private: \
    type( type&& ); \
    type& operator=( type&& )

//!  意図した未使用変数に対するコンパイラ警告を抑制
#define TS_UNUSED( variable ) \
    ( void )( variable )

//! 対象の利用は非推奨であることを宣言
#define TS_DEPRECATED \
    __declspec(deprecated("非推奨な項目を利用しています。この機能は廃止予定です。"))

//!  現在の関数の名前
#define TS_FUNCTION __FUNCTION__

//! 現在のファイルの名前
#define TS_FILENAME __FILE__

//! 現在の行数
#define TS_LINE __LINE__

//!  関数が例外を投げないことを宣言
#define TS_NOEXCEPT throw()

//! ログを出力します。
#define TS_LOG(...)\
    {\
        TS::LogMetaData metadata; \
        metadata.line = TS_LINE; \
        metadata.fileName = TS_FILENAME; \
        metadata.functionName = TS_FUNCTION; \
        metadata.logLevel = TS::Log_Info; \
        TS::GetLogger().Log(metadata, __VA_ARGS__);\
    }

//! デバッグログを出力します。
#ifdef _DEBUG
    #define TS_LOG_DEBUG(...)\
        {\
            TS::LogMetaData metadata; \
            metadata.line = TS_LINE; \
            metadata.fileName = TS_FILENAME; \
            metadata.functionName = TS_FUNCTION; \
            metadata.logLevel = TS::Log_Debug; \
            TS::GetLogger().Log(metadata, __VA_ARGS__);\
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
        TS::GetLogger().Log(metadata, __VA_ARGS__);\
    }

//! エラーログを出力します。
#define TS_LOG_ERROR(...)\
    {\
        TS::LogMetaData metadata; \
        metadata.line = TS_LINE; \
        metadata.fileName = TS_FILENAME; \
        metadata.functionName = TS_FUNCTION; \
        metadata.logLevel = TS::Log_Error; \
        TS::GetLogger().Log(metadata, __VA_ARGS__);\
    }
