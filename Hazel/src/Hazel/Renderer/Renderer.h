#pragma once

#include "RenderCommand.h"
#include "Shader.h"
#include "Core/OrhographicCamera.h"

namespace Hazel
{
	class Renderer
	{
	public:
		inline static void Init() { RenderCommand::Init(); }
		inline static void OnWindowResize(uint32_t width, uint32_t height) { RenderCommand::SetViewport(0, 0, width, height); }

		static void BeginScene(OrhographicCamera& camera); // TODO it's had to be implemented
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}