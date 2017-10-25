#include "File.h"
#include <iostream>
inline const char* TS::File::GetPath() const
{
    return m_path;
}

TS::File::File(const char* path)
{
    memcpy(m_path, path,sizeof(path));
}

TS::Folder::Folder(const char* path):File(path)
{
}
