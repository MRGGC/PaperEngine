#include "p_pch.h"

#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Paper
{
	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const char* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, NULL);

		glCompileShader(vertexShader);

		int is_compiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &is_compiled);

		if (!is_compiled)
		{
			int len;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &len);

			char* msg = (char*)alloca(len * sizeof(char));
			glGetShaderInfoLog(vertexShader, len, &len, msg);

			glDeleteShader(vertexShader);

			PAPER_CORE_ERROR("{0}", msg);
			PAPER_CORE_ASSERT(is_compiled, "Vertex Shader compilation Error!");

			return;
		}

		// Fragment Shader //
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, NULL);

		glCompileShader(fragmentShader);

		is_compiled = 0;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &is_compiled);

		if (!is_compiled)
		{
			int len;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &len);

			char* msg = (char*)alloca(len * sizeof(char));
			glGetShaderInfoLog(fragmentShader, len, &len, msg);

			glDeleteShader(fragmentShader);

			PAPER_CORE_ERROR("{0}", msg);
			PAPER_CORE_ASSERT(is_compiled, "Fragment Shader compilation Error!");

			return;			
		}

		// Program Creation //
		unsigned int program = glCreateProgram();

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		glLinkProgram(program);

		int is_linked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &is_linked);

		if (!is_linked)
		{
			int len;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

			char* msg = (char*)alloca(len * sizeof(char));
			glGetProgramInfoLog(program, len, &len, msg);

			glDeleteProgram(program);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			PAPER_CORE_ERROR("{0}", msg);
			PAPER_CORE_ASSERT(is_linked, "Shader Program linking Error!");

			return;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		m_RendererID = program;
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::SetUniformInt(const std::string& name, int value)
	{
		GLint loc = glGetUniformLocation(m_RendererID, name.c_str()); 
		glUniform1i(loc, value);
	}

	void OpenGLShader::SetUniformFloat(const std::string& name, float value)
	{
		GLint loc = glGetUniformLocation(m_RendererID, name.c_str()); 
		glUniform1f(loc, value);
	}

	void OpenGLShader::SetUniformFloat2(const std::string& name, const glm::vec2& vec)
	{
		GLint loc = glGetUniformLocation(m_RendererID, name.c_str()); 
		glUniform2f(loc, vec.x, vec.y);
	}

	void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& vec)
	{
		GLint loc = glGetUniformLocation(m_RendererID, name.c_str()); 
		glUniform3f(loc, vec.x, vec.y, vec.z);
	}

	void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& vec)
	{
		GLint loc = glGetUniformLocation(m_RendererID, name.c_str()); 
		glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint loc = glGetUniformLocation(m_RendererID, name.c_str()); 
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint loc = glGetUniformLocation(m_RendererID, name.c_str()); 
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}
}