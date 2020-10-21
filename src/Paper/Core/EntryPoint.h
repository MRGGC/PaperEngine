#pragma once
#include "Paper/Core/Core.h"

extern Paper::Application* Paper::CreateApplication();

int main(int argc, const char* argv[])
{
	Paper::Log::Init();

	PAPER_PROFILE_BEGIN_SESSION("Startup", "PaperProfile-Startup.json");
	auto app = Paper::CreateApplication();
	PAPER_PROFILE_END_SESSION();

	PAPER_PROFILE_BEGIN_SESSION("Runtime", "PaperProfile-Runtime.json");
	app->Run();
	PAPER_PROFILE_END_SESSION();

	PAPER_PROFILE_BEGIN_SESSION("Shutdown", "PaperProfile-Shutdown.json");
	delete app;
	PAPER_PROFILE_END_SESSION();
	return 0;
}