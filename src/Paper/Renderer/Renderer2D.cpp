#include "p_pch.h"
#include "Renderer2D.h"

#include "Paper/Core/Application.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "TextRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Paper
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	struct Renderer2DStorage
	{
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVerticies = MaxQuads * 4;
		static const uint32_t MaxIndicies = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: Retrieve from the GPU

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;

		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;		
	};

	static Renderer2DStorage s_Data;

	void Renderer2D::Init()
	{
		PAPER_PROFILE_FUNCTION();

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVerticies * sizeof(QuadVertex));

		BufferLayout SquareBufferLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" }
		};
		s_Data.QuadVertexBuffer->SetLayout(SquareBufferLayout);
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVerticies];

		uint32_t* quad_indices = new uint32_t[s_Data.MaxIndicies];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndicies; i += 6)
		{
			quad_indices[i + 0] = offset + 0;
			quad_indices[i + 1] = offset + 1;
			quad_indices[i + 2] = offset + 2;

			quad_indices[i + 3] = offset + 2;
			quad_indices[i + 4] = offset + 3;
			quad_indices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> QuadIBO = IndexBuffer::Create(quad_indices, s_Data.MaxIndicies);
		s_Data.QuadVertexArray->SetIndexBuffer(QuadIBO);
		delete[] quad_indices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xFFFFFFFF;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.TextureShader = Shader::Create("Sandbox/assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		PAPER_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		PAPER_PROFILE_FUNCTION();

		s_Data.QuadIndexCount = 0;

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		PAPER_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
			s_Data.TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::ResetBatch()
	{
		EndScene();

		s_Data.QuadIndexCount = 0;

		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	} 

	// Primitives
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({position.x, position.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		PAPER_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndicies)
			ResetBatch();

		constexpr float texIndex = 0.0f; // white texture
		constexpr float tilingFactor = 1.0f;

		constexpr size_t quadVertexCount = 4;

		glm::vec3 positions[4] = {
			{ position.x - size.x/2, position.y - size.y/2, position.z },
			{ position.x + size.x/2, position.y - size.y/2, position.z },
			{ position.x + size.x/2, position.y + size.y/2, position.z },
			{ position.x - size.x/2, position.y + size.y/2, position.z }
		};

		constexpr glm::vec2 textureCoords[quadVertexCount] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		for (uint32_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = positions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float angle)
	{
		DrawRotatedQuad({position.x, position.y, 0.0f}, size, color, angle);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float angle)
	{
		PAPER_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndicies)
			ResetBatch();

		constexpr float texIndex = 0.0f; // white texture
		constexpr float tilingFactor = 1.0f;
		constexpr size_t quadVertexCount = 4;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
		                      glm::rotate(glm::mat4(1.0f), angle, {0.0f, 0.0f, 1.0f}) *
							  glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		constexpr glm::vec2 textureCoords[quadVertexCount] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		for (uint32_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({position.x, position.y, 0.0f}, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad(position, size, texture, 1.0f, glm::vec4(1.0f));
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tint)
	{
		PAPER_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndicies)
			ResetBatch();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float) i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			if (s_Data.TextureSlotIndex == s_Data.MaxTextureSlots) {
				ResetBatch();
			}
			textureIndex = (float) s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		constexpr size_t quadVertexCount = 4;

		glm::vec3 positions[quadVertexCount] = {
			{ position.x - size.x/2, position.y - size.y/2, position.z },
			{ position.x + size.x/2, position.y - size.y/2, position.z },
			{ position.x + size.x/2, position.y + size.y/2, position.z },
			{ position.x - size.x/2, position.y + size.y/2, position.z }
		};

		constexpr glm::vec2 textureCoords[quadVertexCount] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		for (uint32_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = positions[i];
			s_Data.QuadVertexBufferPtr->Color = tint;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture)
	{
		DrawQuad({position.x, position.y, 0.0f}, size, subtexture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture)
	{
		DrawQuad(position, size, subtexture, 1.0f, glm::vec4(1.0f));
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& tint)
	{
		PAPER_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndicies)
			ResetBatch();

		float textureIndex = 0.0f;
		constexpr size_t quadVertexCount = 4;
		const glm::vec2* textureCoords = subtexture->GetTexCoords();
		const Ref<Texture2D> texture = subtexture->GetTexture();

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float) i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			if (s_Data.TextureSlotIndex == s_Data.MaxTextureSlots) {
				ResetBatch();
			}
			textureIndex = (float) s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::vec3 positions[quadVertexCount] = {
			{ position.x - size.x/2, position.y - size.y/2, position.z },
			{ position.x + size.x/2, position.y - size.y/2, position.z },
			{ position.x + size.x/2, position.y + size.y/2, position.z },
			{ position.x - size.x/2, position.y + size.y/2, position.z }
		};

		for (uint32_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = positions[i];
			s_Data.QuadVertexBufferPtr->Color = tint;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float angle)
	{
		DrawRotatedQuad({position.x, position.y, 0.0f}, size, texture, angle);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float angle)
	{
		DrawRotatedQuad({position.x, position.y, 0.0f}, size, texture, angle, 1.0f, glm::vec4(1.0f));
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float angle, float tilingFactor, const glm::vec4& tint)
	{
		DrawRotatedQuad({position.x, position.y, 0.0f}, size, texture, angle, tilingFactor, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float angle, float tilingFactor, const glm::vec4& tint)
	{
		PAPER_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndicies)
			ResetBatch();

		float textureIndex = 0.0f;
		constexpr size_t quadVertexCount = 4;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float) i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			if (s_Data.TextureSlotIndex == s_Data.MaxTextureSlots) {
				ResetBatch();
			}
			textureIndex = (float) s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
		                      glm::rotate(glm::mat4(1.0f), angle, {0.0f, 0.0f, 1.0f}) *
							  glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		constexpr glm::vec2 textureCoords[quadVertexCount] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		for (uint32_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tint;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float angle)
	{
		DrawRotatedQuad({position.x, position.y, 0.0f}, size, subtexture, angle);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float angle)
	{
		DrawRotatedQuad(position, size, subtexture, angle, 1.0f, glm::vec4(1.0f));
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float angle, float tilingFactor, const glm::vec4& tint)
	{
		DrawRotatedQuad({position.x, position.y, 0.0f}, size, subtexture, angle, 1.0f, glm::vec4(1.0f));
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float angle, float tilingFactor, const glm::vec4& tint)
	{
		PAPER_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndicies)
			ResetBatch();

		float textureIndex = 0.0f;
		constexpr size_t quadVertexCount = 4;
		const glm::vec2* textureCoords = subtexture->GetTexCoords();
		const Ref<Texture2D> texture = subtexture->GetTexture();

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float) i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			if (s_Data.TextureSlotIndex == s_Data.MaxTextureSlots) {
				ResetBatch();
			}
			textureIndex = (float) s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
		                      glm::rotate(glm::mat4(1.0f), angle, {0.0f, 0.0f, 1.0f}) *
							  glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		for (uint32_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tint;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}
	
	void Renderer2D::WriteText(const std::string& text, const glm::vec3& position, float scale, const glm::vec4& color)
	{
		const Ref<Texture2D>& FontAtlas = TextRenderer::GetTexture();

		const float k = 0.01f;
		scale *= k;

		float offsetX = 0.0f;
		for (uint32_t i = 0; i < text.length(); i++) {
			unsigned char c = text[i];
			GlyphInfo& Character = TextRenderer::GetGlyphInfos()[(int)c];

			glm::vec2 min = { Character.Top.x / FontAtlas->GetWidth(), (FontAtlas->GetHeight() - Character.Bottom.y) / FontAtlas->GetHeight() };
			glm::vec2 max = { Character.Bottom.x / FontAtlas->GetWidth(), (FontAtlas->GetHeight() - Character.Top.y) / FontAtlas->GetHeight() };
			Ref<SubTexture2D> CharTexture = CreateRef<SubTexture2D>(FontAtlas, min, max);

			float w = Character.Bottom.x - Character.Top.x, h = Character.Bottom.y - Character.Top.y;
			glm::vec2 size = { w * scale, h * scale };

			glm::vec3 newPos = {scale * (position.x + w/2 + Character.Offset.x + offsetX), scale * (position.y + h/2 + (Character.Offset.y - h)), position.z};

			offsetX += Character.Advance;

			DrawQuad(newPos, size, CharTexture, 1.0f, color);
		}
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Renderer2D::Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}
}
