#include "p_pch.h"

#include "Renderer.h"
#include "Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Paper
{
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		PAPER_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjectionMatrix", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_ModelMatrix", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}