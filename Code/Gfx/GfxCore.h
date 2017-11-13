#pragma once
#include "GfxDevice.h"

namespace TS
{
    class GfxCore :public  Object
    {
    private:
        SharedPtr<GfxDevice> m_pDevice;
    public:
        /**
         * \brief コンストラクタ
         * \param windowHandle 
         */
        GfxCore( HWND windowHandle );
        /**
         * \brief シーン開始
         */
        void BeginScene();


        void ClearColor(float r, float g, float b, float a);

        void SetVertexShader(void * vertexShader)
        {
            //todo;
        }
        void SetPixelShader(void  * pixelShader)
        {
            //todo;
        }

        template<class T>
        void SetConstantValue(const char* variableName, T& value)
        {
            //todo;
        }

        void SetTopology(Topology topology)
        {
            //todo;
        }

        void SetVertexBuffer(void * )
        {
            //todo;
        }

        void SetIndexBuffer(void *)
        {
            //todo;
        }

        void Draw()
        {
            //todo;
        }

        void Draw(int startPrimtive,int endPrimitive)
        {
            //todo;            
        }

        void DrawIndex()
        {
            //todo;
        }

        void SetRenderTarget(int index , void * rt)
        {
            
        }

        void UnRenderTarget(int index )
        {

        }

        /**
         * \brief シーン終了
         */
        void EndScene();
    };

}
