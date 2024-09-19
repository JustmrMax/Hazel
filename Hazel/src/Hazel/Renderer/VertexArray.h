#pragma once

#include <memory>
#include <vector>

#include "Buffer.h"

namespace Hazel
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual inline std::vector<Ref<VertexBuffer>>& GetVertexBuffers() = 0;
		virtual inline Ref<IndexBuffer>& GetIndexBuffer() = 0;

		static VertexArray* Create();
	};
}