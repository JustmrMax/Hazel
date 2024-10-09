#include "hzpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>	

namespace Hazel
{
	static unsigned int ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		else if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		HZ_CORE_ASSERT(false, "Not appropirate shader type - '{0}'!", type);
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		std::unordered_map<unsigned int, std::string> shaderSources = PreProcess(source);
		Compile(shaderSources);

		ExtractName(filepath, m_Name);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		std::unordered_map<unsigned int, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	const std::string& OpenGLShader::GetName() const
	{
		return m_Name;
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, const int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, const float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());

			in.close();
		}
		else
			HZ_CORE_WARN("Shader File '{0}' could not be opended!", filepath);

		return result;
	}

	std::unordered_map<unsigned int, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<unsigned int, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			HZ_CORE_ASSERT((eol != std::string::npos), "Syntax error!");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			HZ_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specification!");

			size_t nextLinePos = source.find_first_not_of("/r/n", eol);
			pos = source.find(typeToken, nextLinePos);

			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos,
				pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(std::unordered_map<unsigned int, std::string>& shaderSources)
	{
		unsigned int program = glCreateProgram();
		// std::vector<unsigned int> glShaderIDs(shaderSources.size());
		HZ_CORE_ASSERT(shaderSources.size() <= 2, "Only support for 2 shader type!");
		int glShaderIDsIndex = 0;
		std::array<unsigned int, 2> glShaderIDs;

		for (auto& keyValue : shaderSources)
		{
			unsigned int type = keyValue.first;
			const std::string& source = keyValue.second;

			unsigned int shader = glCreateShader(type);

			const char* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			int isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if (isCompiled == 0)
			{
				int maxLength = 0;

				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				HZ_CORE_ERROR("{0}", infoLog.data());
				HZ_CORE_ASSERT(false, "Shader Compilation failure!");

				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDsIndex++] = shader;
			// glShaderIDs.push_back(shader);
		}

		glLinkProgram(program);

		int isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

		if (isLinked == 0)
		{
			int maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			for (unsigned int id : glShaderIDs)
				glDeleteShader(id);

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Shader Link failure!");
			return;
		}

		for (unsigned int id : glShaderIDs)
			glDetachShader(program, id);

		m_RendererID = program;
	}

	void OpenGLShader::ExtractName(const std::string& filepath, std::string& result)
	{
		// Extracting name from filepath
		size_t strSize = filepath.size();

		// 1. Find last slash in filepath
		size_t lastSlashPos = filepath.rfind("\\");

		if (lastSlashPos == std::string::npos)
			lastSlashPos = filepath.rfind("/");

		HZ_CORE_ASSERT((lastSlashPos != std::string::npos), "Invalid filepath!");

		// 2. Find Last dot
		size_t lastDotPos = filepath.rfind('.');
		size_t extensionLen;

		if (lastDotPos != std::string::npos)
			extensionLen = strSize - lastDotPos + 1;
		else
			extensionLen = 0;

		// 3. Extracting name
		size_t textureNameCount = strSize - lastSlashPos - extensionLen;
		result = filepath.substr(lastSlashPos + 1, textureNameCount);
	}
}