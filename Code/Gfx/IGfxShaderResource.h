#pragma once

namespace TS
{
    class IGfxShaderResource : public IGfxResource
    {
    public:
        IGfxShaderResource();;
        void SetRegisterID(const int id);
        int GetRegisterID() const;
    private:
        int m_registerID;
    };
}

