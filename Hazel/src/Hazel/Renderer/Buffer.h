#pragma once

#include "Renderer.h"
#include <string>

namespace Hazel
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static VertexBuffer* CreateBuffer(float* vertices, uint32_t count);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* CreateBuffer(uint32_t *indecies, uint32_t count);
	};
}