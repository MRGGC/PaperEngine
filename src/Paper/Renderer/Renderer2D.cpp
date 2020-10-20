#include "p_pch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"

#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Paper
{

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
		Ref<Shader> TextureShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		s_Data->QuadVertexArray = VertexArray::Create();

		float square_vertecies[5*4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVBO;
		squareVBO.reset(VertexBuffer::Create(square_vertecies, sizeof(square_vertecies)));
		BufferLayout SquareBufferLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		};
		squareVBO->SetLayout(SquareBufferLayout);
		s_Data->QuadVertexArray->AddVertexBuffer(squareVBO);

		uint32_t square_indices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIBO;
		squareIBO.reset(IndexBuffer::Create(square_indices, sizeof(square_indices)/sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIBO);

		s_Data->FlatColorShader = Shader::Create("Sandbox/assets/shaders/FlatColor.glsl");
		s_Data->TextureShader = Shader::Create("Sandbox/assets/shaders/Texture.glsl");

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	// Primitives
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const float& angle)
	{
		DrawQuad({position.x, position.y, 0.0f}, size, color, angle);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const float& angle)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
		                      glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f)) * 
		                      glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->FlatColorShader->SetMat4("u_ModelMatrix", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, const float& angle)
	{
		DrawQuad({position.x, position.y, 0.0f}, size, texture, angle);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, const float& angle)
	{
		s_Data->TextureShader->Bind();

		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
		                      glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f)) * 
		                      glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_ModelMatrix", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}