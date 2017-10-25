#pragma once
#include "../../Core/Object.h"
namespace TS
{
    class File : public Object
    {
    protected:
        char m_path[255];
    public:
        const char* GetPath() const;
        const char* GetFullPath() const;

        File(const char * path);
    };

    class Folder : public File
    {
    public:
        Folder(const char* path);
    };
}
