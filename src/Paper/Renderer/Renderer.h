#pragma once

#include "RenderCommand.h"

namespace Paper
{
	class PAPER_API Renderer
	{
	public:
		// TODO: Take Scene Parameters
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
	public:
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}