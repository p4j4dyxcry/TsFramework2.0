#define TS_ENGINE_METHOD_OVERRIDE
#include "Code\Core\Engine.h"

void RunTests();


void main(int argc, char *argv[])
{
	auto engine = TS::Engine::Create();

    RunTests();

    while (engine->IsRuning())
	{
        engine->UpdateEngine();
	}
	engine->Destory();
}
