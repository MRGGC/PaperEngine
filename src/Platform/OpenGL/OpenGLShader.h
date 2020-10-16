#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Paper/Renderer/Shader.h"

namespace Paper
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void SetUniformInt(const std::string& name, int value);

		void SetUniformFloat(const std::string& name, float value);
		void SetUniformFloat2(const std::string& name, const glm::vec2& vec);
		void SetUniformFloat3(const std::string& name, const glm::vec3& vec);
		void SetUniformFloat4(const std::string& name, const glm::vec4& vec);

		void SetUniformMat3(const std::string& name, const glm::mat3& matrix);
		void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		uint32_t m_RendererID;
	};
}