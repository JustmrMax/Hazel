#include "hzpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLShader.h"

namespace Hazel
{
	Ref<Shader> Shader::Create(const std::string& pathFile)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "There is handle for None rendererAPI");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(pathFile);
		}

		HZ_CORE_ASSERT(false, "There is no such a rendererAPI");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, std::string& vertexSrc, std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "There is handle for None rendererAPI");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		HZ_CORE_ASSERT(false, "There is no such a rendererAPI");
		return nullptr;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//ShaderLibrary//////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		HZ_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		const std::string& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& pathfile)
	{
		Ref<Shader> shader = Shader::Create(pathfile);
		Add(shader); // RefCount += 1

		return shader; // RefCount += 1
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& pathfile)
	{
		Ref<Shader> shader = Shader::Create(pathfile);
		Add(name, shader); // RefCount += 1

		return shader; // RefCount += 1

	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		HZ_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name]; // RefCount += 1
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}