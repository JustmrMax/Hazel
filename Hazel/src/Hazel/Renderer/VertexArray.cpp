#include "hzpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLVertexArray.h"

namespace Hazel
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "There is handle for None rendererAPI");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		}

		HZ_CORE_ASSERT(false, "There is no such a rendererAPI");
		return nullptr;
	}
}