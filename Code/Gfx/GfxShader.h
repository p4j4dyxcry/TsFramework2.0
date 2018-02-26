#pragma once

namespace TS
{
	class GfxShader : public IGfxResource
	{
    public :
	    GfxShader():IGfxResource(){}

        bool LoadShaderFromFile();
	};
}