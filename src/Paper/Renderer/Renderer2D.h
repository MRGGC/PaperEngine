#pragma once

#include "Paper/Renderer/OrthographicCamera.h"
#include "Paper/Renderer/Texture.h"

namespace Paper
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const float& angle = 0.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const float& angle = 0.0f);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, const float& angle = 0.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, const float& angle = 0.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, const glm::vec4& tint, const float& angle = 0.0f);
	};
}