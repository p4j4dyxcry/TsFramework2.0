#pragma once

namespace TS
{
	class FileUtility
	{
	public :
	    /**
		 * \brief 指定されたパスがフルパスかどうかを判断
		 * \param path 
		 * \return フルパスならtrue
		 */
		static bool IsFullPath(const char * path);		


	    /**
		 * \brief 指定されたファイルのファイルサイズを取得
		 * \param path 
		 * \return 存在しない場合は -1
		 */
		static int  FileSize(const char* path);


	    /**
		 * \brief ファイル、またはディレクトリが存在するかをチェック
		 * \param path 
		 * \return 
		 */
		static bool Exist(const char* path);


	    /**
		 * \brief ローカルパスをフルパスに変換します
		 * \param dest 戻り値
		 * \param in   入力
		 */
		static void ToFullPath(char * dest, const char * in);


	    /**
		 * \brief 指定されたファイルが存在するディレクトリ名を取得します
		 * \param dest 戻り値
		 * \param in   入力
		 */
		static void GetDirectory(char* dest, const char * in);


	    /**
		 * \brief カレントディレクトリの取得
		 * \param dest 戻り値
		 */
		static void GetCD(char* dest);


	    /**
		 * \brief カレントディレクトリを設定
		 * \param dir 
		 * \return 成功ならtrue
		 */
		static bool SetCD(char* dir);


	    /**
		 * \brief ファイルを指定されたパスにコピーします
		 * \param source コピー元
		 * \param dest   コピー先
		 * \param isOverride 指定されたパスにすでにファイルが存在する場合に上書きするかどうか
		 * \return 成功ならtrue
		 */
		static bool Copy(char* source,const char* dest,bool isOverride = true);

        /**
        * \brief ファイルを指定されたパスに移動します
        * \param source 移動元
        * \param dest   移動先
        * \param isOverride 指定されたパスにすでにファイルが存在する場合に上書きするかどうか
        * \return 成功ならtrue
        */
		static bool Move(char* source, const char* dest,bool isOverride = true);


	    /**
		 * \brief ファイルまたはディレクトリを削除します
		 * \param file ファイルパス
		 * \param isDir ディレクトリかどうか
		 * \return 成功ならtrue
		 */
		static bool Delete(char* file,bool isDir = false);


	    /**
		 * \brief 指定されたパスに新しいディレクトリを生成します
		 * \param dir 
		 * \return trueなら成功
		 */
		static bool CreateDir(char* dir);


	    /**
		 * \brief 指定されたパスを一つ上の階層に変換します
		 * \param dir 入出力
		 * \return trueなら成功
		 */
		static bool ToParentDirectory(char * dir);


	    /**
		 * \brief 指定されたパスを一つ上の階層に変換します
		 * \param dest 戻り値
		 * \param dir  入力
		 * \return trueなら成功
		 */
		static bool GetParentDirectory(char * dest,const char* dir);


	    /**
		 * \brief 2つのパスを結合します結果はフルパスになります
		 * \param dest 戻り値
		 * \param path1 
		 * \param path2 
		 * \return trueなら成功
		 */
		static bool Combine(char* dest ,const char* path1, const char*path2);

        /**
        * \brief 2つのパスを結合します結果はフルパスになります
        * \param path1　入出力
        * \param path2
        * \return trueなら成功
        */
		static bool Combine(char* path1, const char* path2);
	    /**
		 * \brief ファイルまたはフォルダのローカル名を取得します
		 * \param dest 出力
		 * \param in 入力
		 */
		static void GetName(char* dest, const char* in);
	};
}