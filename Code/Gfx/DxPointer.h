#pragma once

namespace TS
{
    template<typename T> 
    SharedPtr<T> CreateDxSharePtr(T* pointer)
    {
        return SharedPtr<T>(pointer, [](T* p)
        {
            if(p != nullptr)
            {
                p->Release();
            }
            p = nullptr;
        });
    }
}