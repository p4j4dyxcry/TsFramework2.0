#define TS_ENGINE_METHOD_OVERRIDE
#include "Code\Core\Engine.h"

void RunTests();

bool TS::Engine::OnInitialize()
{
	RunTests();
	return true;
}

void TS::Engine::OnUpdate()
{
	return;
}


bool TS::Engine::OnFinalize()
{
	return true;
}

void main(int argc, char *argv[])
{
	auto engine = TS::Engine::Create();
	
	MSG msg;
	while (engine->IsRuning())
	{
		if (GetMessage(&msg, nullptr, 0, 0) != -1)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	engine->Destory();
}
