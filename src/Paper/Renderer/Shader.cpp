#include "p_pch.h"

#include "Shader.h"

#include <glad/glad.h>

namespace Paper
{
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
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

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}
}