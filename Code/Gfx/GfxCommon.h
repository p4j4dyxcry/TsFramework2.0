#pragma once

namespace TS
{
    enum class Topology
    {
        PointList     = D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
        LineList      = D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
        LineStrip     = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
        TriangleList  = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        TriangleStrip = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
    };

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