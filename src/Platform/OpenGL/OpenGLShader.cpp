#include "p_pch.h"

#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Paper
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")    return GL_VERTEX_SHADER;
		if (type == "fragment")  return GL_FRAGMENT_SHADER;
		if (type == "pixel")     return GL_FRAGMENT_SHADER;

		PAPER_CORE_ASSERT(false, "Unknown Shader Type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		PAPER_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;

		Compile(sources);
	}

	OpenGLShader::OpenGLShader(const std::string& path)
	{
		PAPER_PROFILE_FUNCTION();

		std::string	shaderSource = ReadFile(path);
		std::unordered_map<GLenum, std::string> shaderSources = PreProcess(shaderSource);
		Compile(shaderSources);

		// Extract File Name
		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = path.rfind('.');

		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;

		m_Name = path.substr(lastSlash, count);
	}

	OpenGLShader::~OpenGLShader()
	{
		PAPER_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		PAPER_PROFILE_FUNCTION();

		uint32_t program = glCreateProgram();

		PAPER_CORE_ASSERT(shaderSources.size() <= 3, "ShaderSources size is greater than 3!");
		std::array<GLenum, 3> glShaderIDs;

		int glShaderIDIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const char* src = source.c_str();
			glShaderSource(shader, 1, &src, NULL);

			glCompileShader(shader);

			GLint is_compiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);

			if (!is_compiled)
			{
				GLint len;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

				char* msg = (char*)alloca(len * sizeof(char));
				glGetShaderInfoLog(shader, len, &len, msg);

				glDeleteShader(shader);

				PAPER_CORE_ERROR("{0}", msg);
				PAPER_CORE_ASSERT(is_compiled, "Shader Compilation Error!");

				return;
			}

			glAttachShader(program, shader);

			glShaderIDs[glShaderIDIndex++] = shader;
		}

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

			for (auto shaderID : glShaderIDs)
				glDeleteShader(shaderID);

			PAPER_CORE_ERROR("{0}", msg);
			PAPER_CORE_ASSERT(is_linked, "Shader Program linking Error!");

			return;
		}

		for (auto shaderID : glShaderIDs)
			glDetachShader(program, shaderID);

		m_RendererID = program;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		PAPER_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			PAPER_CORE_ASSERT(eol != std::string::npos, "Syntax Error!");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);

			GLenum glType = ShaderTypeFromString(type);
			PAPER_CORE_ASSERT(glType, "Invalid Shader Type specified!");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);

			shaderSources[glType] =
				source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		PAPER_PROFILE_FUNCTION();

		std::string result;
		std::ifstream input(filepath, std::ios::in | std::ios::binary);
		if (input) {
			input.seekg(0, std::ios::end);
			result.resize(input.tellg());
			input.seekg(0, std::ios::beg);
			input.read(&result[0], result.size());

			input.close();
		} else {
			PAPER_CORE_ERROR("Couldn't Open file '{0}'", filepath);
		}

		return result;
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint loc = glGetUniformLocation(m_RendererID, name.c_str()); 
		glUniform1i(loc, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint loc = glGetUniformLocation(m_RendererID, name.c_str()); 
		glUniform1f(loc, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& vec)
	{
		GLint loc = glGetUniformLocation(m_RendererID, name.c_str()); 
		glUniform2f(loc, vec.x, vec.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vec)
	{
		GLint loc = glGetUniformLocation(m_RendererID, name.c_str()); 
		glUniform3f(loc, vec.x, vec.y, vec.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& vec)
	{
		GLint loc = glGetUniformLocation(m_RendererID, name.c_str()); 
		glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint loc = glGetUniformLocation(m_RendererID, name.c_str()); 
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint loc = glGetUniformLocation(m_RendererID, name.c_str()); 
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		PAPER_PROFILE_FUNCTION();

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		PAPER_PROFILE_FUNCTION();

		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		PAPER_PROFILE_FUNCTION();

		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		PAPER_PROFILE_FUNCTION();

		UploadUniformMat4(name, value);
	}

	void OpenGLShader::Bind() const
	{
		PAPER_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		PAPER_PROFILE_FUNCTION();

		glUseProgram(0);
	}
}