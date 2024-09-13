#pragma once

#include "RenderCommand.h"
#include "Shader.h"
#include "OrhographicCamera.h"

namespace Hazel
{
	class Renderer
	{
	public:
		static void BeginScene(OrhographicCamera& camera); // TODO it's had to be implemented
		static void EndScene();

		static void Submit(const std::unique_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}