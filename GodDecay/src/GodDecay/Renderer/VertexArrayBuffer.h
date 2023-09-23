#pragma once

#include "GodDecay/Core.h"
#include "Buffer.h"

/// <summary>
/// 对VertexArray的高层抽象
/// 在此类中对布局的buufer进行属性的赋值
/// </summary>
namespace GodDecay 
{
	class VertexArrayBuffer
	{
	public:
		virtual ~VertexArrayBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& IndqexBuffer) = 0;

		virtual std::vector<Ref<VertexBuffer>> GetVertexBuffers() const = 0;
		virtual Ref<IndexBuffer> GetIndexBuffer() const = 0;

		static Ref<VertexArrayBuffer> Create();
	};
}



