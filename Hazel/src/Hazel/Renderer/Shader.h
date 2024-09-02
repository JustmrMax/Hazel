#pragma once

#include <string>

namespace Hazel
{
	class Shader
	{
	public:
		Shader(std::string &vertexSrc, std::string &fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

	private:
		uint32_t m_RendererID = 0;
	};
}