#include "hzpch.h"
#include "Renderer.h"

namespace Hazel
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrhographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::unique_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		RenderCommand::DrawIndexed(vertexArray);
	}

}