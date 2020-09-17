#pragma once

#include "Core.h"

namespace Paper
{
	class PAPER_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// CLIENT PART
	PAPER_API Application* CreateApplication();
}