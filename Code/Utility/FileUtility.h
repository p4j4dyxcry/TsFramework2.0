#pragma once

namespace TS
{
	class FileUtility
	{
	public :
		static bool IsFullPath(const char * path);		
		static int  FileSize(const char* path);
		static bool Exist(const char* path);
		static void ToFullPath(char * dest, const char * in);
		static void GetDirectory(char* dest, const char * in);
		static void GetCD(char* dest);
		static bool SetCD(char* dir);
		static bool Copy(char* source,const char* dest,bool isOverride = true);
		static bool Move(char* source, const char* dest,bool isOverride = true);
		static bool Delete(char* file,bool isDir = false);
		static bool CreateDir(char* dir);
		static bool ToParentDirectory(char * dir);
		static bool GetParentDirectory(char * dest,const char* dir);
		static bool Combine(char* dest ,const char* path1, const char*path2);
		static bool Combine(char* path1, const char* path2);
		static void GetName(char* dest, const char* in);
	};
}