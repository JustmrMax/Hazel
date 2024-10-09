#include "hzpch.h"
#include "Renderer.h" // YEAP depedency circle

#include "Platforms/OpenGL/OpenGLBuffer.h"

namespace Hazel
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//BufferLayout////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BufferLayout::BufferLayout()
	{
	}

	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
		: m_Elemenets(elements)
	{
		CalculateOffsetAndStride();
	}

	BufferLayout::~BufferLayout()
	{
	}

	void BufferLayout::CalculateOffsetAndStride()
	{
		uint32_t offset = 0;
		m_Stride = 0;

		for (auto& element : m_Elemenets)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//VertexBuffer/////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Ref<VertexBuffer> VertexBuffer::Create(float *vertices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "There is handle for None rendererAPI");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(vertices, count);
		}

		HZ_CORE_ASSERT(false, "There is no such a rendererAPI");
		return nullptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//IndexBuffer///////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indecies, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "There is handle for None rendererAPI");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(indecies, count);
		}

		HZ_CORE_ASSERT(false, "There is no such a rendererAPI");
		return nullptr;
	}
}