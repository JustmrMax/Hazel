#pragma once

#include "Hazel/Renderer/Buffer.h"

namespace Hazel
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float *vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual inline BufferLayout GetLayout() const override { return m_Layout; }
		virtual inline void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

	private:
		uint32_t m_RendererID = 0;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t *indecies, uint32_t count);
		virtual ~OpenGLIndexBuffer();	

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint32_t GetCount() const override;

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Count = 0;
	};
}