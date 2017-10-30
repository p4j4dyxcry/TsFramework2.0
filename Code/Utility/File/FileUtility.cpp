#include "FileUtility.h"
#include <sys\stat.h>
#include <windows.h>
#include <direct.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")


namespace TS
{
	bool FileUtility::IsFullPath(const char* path)
	{		
		return std::string(path).find(':') > 0;
	}

	int FileUtility::FileSize(const char* path)
	{
		struct stat statBuf;
		return stat(path, &statBuf);
	}

	bool FileUtility::Exist(const char* path)
	{	
		return FileSize(path) >= 0;
	}
	void FileUtility::ToFullPath(char * dest,const char * in)
	{
		::_fullpath(dest, in, _MAX_PATH);
	}

	void FileUtility::GetDirectory(char* dest, const char * in)
	{
		ToFullPath(dest, in);
		PathRemoveFileSpec(dest);
	}

	void FileUtility::GetCD(char* dest)
	{
		GetCurrentDirectory(_MAX_PATH,dest);
	}
	bool FileUtility::SetCD(char* dir)
	{
		return SetCurrentDirectory(dir) > 0;
	}

	bool FileUtility::Copy(char* source, const char* dest ,bool isOverride)
	{		
		return CopyFile(source, dest, isOverride) > 0;
	}
	bool FileUtility::Move(char* source, const char* dest, bool isOverride )
	{
		DWORD flag = MOVEFILE_COPY_ALLOWED;
		if (isOverride)
			flag &= MOVEFILE_REPLACE_EXISTING;
		return MoveFileEx(source, dest, flag) > 0;
	}
	bool FileUtility::Delete(char* source , bool isDir)
	{
		if(isDir)
			return RemoveDirectory(source) > 0;
		return DeleteFile(source) > 0;
	}

	bool FileUtility::CreateDir(char* dir)
	{
		return _mkdir(dir) == 0;
	}
	bool FileUtility::ToParentDirectory(char * dir)
	{
		return Combine(dir, "/../");
	}
	bool FileUtility::GetParentDirectory(char * dest,const char* source)
	{
		return Combine(dest, source, "/../");
	}

	bool FileUtility::Combine(char * path0 , const char * path2)
	{
		return Combine(path0,path0,path2);
	}

	bool FileUtility::Combine(char * dest ,const char * path0, const char * path2)
	{
		ToFullPath(dest, std::string(path0).append("\\").append(path2).c_str());
		return true;
	}

	void FileUtility::GetName(char * dest, const char * in)
	{
		GetDirectory(dest,in);
		strcpy_s(dest, _MAX_PATH,PathFindFileName(dest));
	}
}