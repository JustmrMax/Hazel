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

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;
		virtual inline std::vector<Ref<VertexBuffer>>& GetVertexBuffers() override { return m_VertexBuffers; }
		virtual inline Ref<IndexBuffer>& GetIndexBuffer() override { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID = 0;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}