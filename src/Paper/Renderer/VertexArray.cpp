#include "p_pch.h"

#include "Renderer.h"

#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Paper
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: {
				PAPER_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
			case RendererAPI::API::OpenGL: {
				return new OpenGLVertexArray();
			}
		}

		PAPER_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}