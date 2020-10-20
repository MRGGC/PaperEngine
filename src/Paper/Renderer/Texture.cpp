#include "p_pch.h"

#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Paper
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: {
				PAPER_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
			case RendererAPI::API::OpenGL: {
				return CreateRef<OpenGLTexture2D>(width, height);
			}
		}

		PAPER_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: {
				PAPER_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
			case RendererAPI::API::OpenGL: {
				return CreateRef<OpenGLTexture2D>(path);
			}
		}

		PAPER_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}