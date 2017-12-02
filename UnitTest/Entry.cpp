#define TS_ENGINE_METHOD_OVERRIDE
#include "Code\Core\Engine.h"

void RunTests();

TS::EngineSetting LoadSetting(int argc, char *argv[])
{
    TS::EngineSetting option;
    const char * filename = "EngineSetting.xml";
    std::ifstream ifs(filename);
    if(ifs.fail())
    {
        std::ofstream ofs(filename);
        cereal::XMLOutputArchive archive(ofs);
        archive(cereal::make_nvp("EngineSetting", option));
    }
    else
    {
        cereal::XMLInputArchive archive(ifs);
        archive(cereal::make_nvp("EngineSetting", option));
    }
    return option;
}

void TestRender(TS::SharedPtr<TS::GfxCore>& gfxSystem)
{

    static float c[3] = { 0,0,0 };

    static int i = 0;

    gfxSystem->BeginScene();

    gfxSystem->ClearColor(c[0], c[1], c[2], 1);

    if (i< 3)
    {
        c[i] += (float)TS::GetEngine()->GetTimeSystem()->GetDeltaTime() * 0.25;

        if (c[i] > 1)
        {
            c[i] = 1;
            i++;
        }
    }

    gfxSystem->EndScene();
}

void main(int argc, char *argv[])
{

    auto option = LoadSetting(argc,argv);

	auto engine = TS::Engine::Create(option);

    RunTests();

    while (engine->IsRuning())
	{
        engine->UpdateEngine();

        TestRender(engine->GetGfxSystem());

        engine->EndFrame();
	}
	engine->Destory();
}
