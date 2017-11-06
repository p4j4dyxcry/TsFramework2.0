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
                TS_LOG_DEBUG("%s")
                p->Release();
            }
            p = nullptr;
        });
    }
}