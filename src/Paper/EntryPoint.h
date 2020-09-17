#pragma once

extern Paper::Application* Paper::CreateApplication();

int main(int argc, const char* argv[])
{

	auto app = Paper::CreateApplication();
	app->Run();

	delete app;
	return 0;
}