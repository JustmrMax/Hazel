#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

#include "Core/Core.h"
	
namespace Hazel
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniformInt(const std::string& name, int value) = 0;

		virtual void SetUniformMat3(const std::string& name, const glm::mat3& matrix) = 0;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;

		virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) = 0;

		virtual const std::string& GetName() const = 0;

	public:
		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, std::string& vertexSrc, std::string& fragmentSrc);
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& pathfile);
		Ref<Shader> Load(const std::string& name, const std::string& pathfile);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders{};
	};
}