#pragma once

#include "Paper/Renderer/OrthographicCamera.h"

#include "Paper/Renderer/Texture.h"
#include "Paper/Renderer/SubTexture2D.h"

namespace Paper
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		// Primitives (rotations in radians)
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, bool ellipsed = false);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, bool ellipsed = false);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float angle, bool ellipsed = false);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float angle, bool ellipsed = false);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, bool ellipsed = false);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, bool ellipsed = false);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tint = glm::vec4(1.0f), bool ellipsed = false);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, bool ellipsed = false);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, bool ellipsed = false);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& tint = glm::vec4(1.0f), bool ellipsed = false);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float angle, bool ellipsed = false);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float angle, bool ellipsed = false);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float angle, float tilingFactor, const glm::vec4& tint = glm::vec4(1.0f), bool ellipsed = false);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float angle, float tilingFactor, const glm::vec4& tint = glm::vec4(1.0f), bool ellipsed = false);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float angle, bool ellipsed = false);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float angle, bool ellipsed = false);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float angle, float tilingFactor, const glm::vec4& tint = glm::vec4(1.0f), bool ellipsed = false);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float angle, float tilingFactor, const glm::vec4& tint = glm::vec4(1.0f), bool ellipsed = false);

		static void DrawTriangle(const std::array<glm::vec3, 3>& positions, const glm::vec4& color);

		// TODO: Think about the API
		static void DrawEllipse(const glm::vec3& position, float radiusX, float radiusY, float angle);

		static void DrawLine(const glm::vec2& pos1, const glm::vec2& pos2, float width, const glm::vec4& color);
		static void DrawLine(const glm::vec3& pos1, const glm::vec3& pos2, float width, const glm::vec4& color);

		// Text
		static void WriteText(const std::string& text, const glm::vec3& position, float scale, const glm::vec4& color);

		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			double QuadCount = 0;

			double GetTotalVertexCount() { return QuadCount * 4; }
			double GetTotalIndexCount() { return QuadCount * 6; }
		};

		static Statistics GetStats();
		static void ResetStats();
	protected:
		static void ResetBatch();
	};
}