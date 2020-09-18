#pragma once

extern Paper::Application* Paper::CreateApplication();

int main(int argc, const char* argv[])
{
	Paper::Log::Init();

	PAPER_CORE_WARN("Initialized Core Log!");

	int a = 20;
	PAPER_CLIENT_ERROR("Ola, Var={0}", a);

	auto app = Paper::CreateApplication();
	app->Run();

	delete app;
	return 0;
}