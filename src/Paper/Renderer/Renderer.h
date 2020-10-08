#pragma once

#include "Paper/Core.h"

namespace Paper
{
	enum class RendererAPI
	{
		None = 0,
		OpenGL = 1
	};

	class PAPER_API Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;
	};
}