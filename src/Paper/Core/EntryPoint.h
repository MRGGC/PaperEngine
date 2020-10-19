#pragma once

extern Paper::Application* Paper::CreateApplication();

int main(int argc, const char* argv[])
{
	Paper::Log::Init();

	PAPER_CORE_INFO("Initialized Core Log!");
	PAPER_INFO("Initialized Client Log!");

	auto app = Paper::CreateApplication();
	app->Run();

	delete app;
	return 0;
}