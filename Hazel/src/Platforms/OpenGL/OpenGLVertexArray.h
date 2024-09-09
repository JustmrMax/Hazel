#pragma once

#include "Hazel/Renderer/VertexArray.h"

namespace Hazel
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
		virtual inline std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() override { return m_VertexBuffers; }
		virtual inline std::shared_ptr<IndexBuffer>& GetIndexBuffer() override { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID = 0;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};
}