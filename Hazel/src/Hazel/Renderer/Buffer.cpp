#include "hzpch.h"
#include "Buffer.h"
#include "Renderer.h"

#include "Platforms/OpenGL/OpenGLBuffer.h"

namespace Hazel
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//VertexBuffer/////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VertexBuffer* VertexBuffer::CreateBuffer(float *vertices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			HZ_CORE_ASSERT(false, "There is handle for None rendererAPI");
			return nullptr;

		case RendererAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, count);
		}

		HZ_CORE_ASSERT(false, "There is no such a rendererAPI");
		return nullptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//IndexBuffer///////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IndexBuffer* IndexBuffer::CreateBuffer(uint32_t *indecies, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			HZ_CORE_ASSERT(false, "There is handle for None rendererAPI");
			return nullptr;

		case RendererAPI::OpenGL:
			return new OpenGLIndexBuffer(indecies, count);
		}

		HZ_CORE_ASSERT(false, "There is no such a rendererAPI");
		return nullptr;
	}
}