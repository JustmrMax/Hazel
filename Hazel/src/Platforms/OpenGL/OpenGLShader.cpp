#include "hzpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>	

namespace Hazel
{
	OpenGLShader::OpenGLShader(const std::string& pathFile)
	{
		// Open source file
		std::string vertexShaderSource, fragmentShaderSource;
		std::ifstream in;
		in.open(pathFile, std::ios::in);

		if (in.is_open())
		{
			// Find #vertex\n

			std::string *currShader = 0;
			std::string currLine;

			while (std::getline(in, currLine))
			{
				if (currLine.find("#vertex") != std::string::npos)
				{
					currShader = &vertexShaderSource;
					continue;
				}
				else if (currLine.find("#fragment") != std::string::npos || currLine.find("pixel") != std::string::npos)
				{
					currShader = &fragmentShaderSource;
					continue;
				}

				if (currShader != 0)
					currShader->append(currLine + "\n");
			}

			in.close();
		}
		else
			HZ_WARN("Can't open shader file '{0}'", pathFile.c_str());

		CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
	}

	OpenGLShader::OpenGLShader(std::string& vertexSrc, std::string& fragmentSrc)
	{
		CreateShaderProgram(vertexSrc, fragmentSrc);
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

	unsigned int OpenGLShader::CompileShader(unsigned int shaderType, const std::string& source)
	{
		// Create an empty vertex shader handle
		unsigned int shader = glCreateShader(shaderType);

		// Send the shader source code to GL
		const char* cStrSource = source.c_str();
		glShaderSource(shader, 1, &cStrSource, 0);

		// Compile the shader
		glCompileShader(shader);

		int isCompiled = 0;

		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == false)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(shader);

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Shader Compilation failure!");

			return 0;
		}
		else
			return shader;
	}

	void OpenGLShader::CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
	{
		unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
		unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

		if (fragmentShader == 0) // Failure compile fragmentShader
			glDeleteShader(vertexShader); // YEAP can be more elagantly

		m_RendererID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);

		// Link our program
		glLinkProgram(m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		int isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);

		if (isLinked == false)
		{
			int maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Program Linking failure!");

			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);

	}
}