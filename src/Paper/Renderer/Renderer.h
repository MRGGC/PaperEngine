#pragma once

#include "Paper/Renderer/RenderCommand.h"

#include "Paper/Renderer/OrthographicCamera.h"
#include "Paper/Renderer/Shader.h"

namespace Paper
{
	class PAPER_API Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		// TODO: Take Scene Parameters
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
	public:
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		
		static SceneData* s_SceneData;
	};
}