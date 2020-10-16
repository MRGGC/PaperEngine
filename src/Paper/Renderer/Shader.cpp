#include "p_pch.h"

#include "Renderer.h"

#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Paper
{
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: {
				PAPER_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
			case RendererAPI::API::OpenGL: {
				return new OpenGLShader(vertexSrc, fragmentSrc);
			}
		}

		PAPER_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}