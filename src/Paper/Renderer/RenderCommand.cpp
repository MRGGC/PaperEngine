#include "p_pch.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Paper
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}