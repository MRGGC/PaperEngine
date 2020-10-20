#include "p_pch.h"

#include "Renderer.h"

#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Paper
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: {
				PAPER_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
			case RendererAPI::API::OpenGL: {
				return std::make_shared<OpenGLVertexArray>();
			}
		}

		PAPER_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}