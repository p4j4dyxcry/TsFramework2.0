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
                TS_LOG_DEBUG("DirectX11 のオブジェクトを開放 , 参照カウント =%d\n", p->AddRef() - 2);
                p->Release();
                p->Release();
            }
            p = nullptr;
        });
    }
}