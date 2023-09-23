#pragma once

#include "GodDecay/Core.h"
#include "Buffer.h"

/// <summary>
/// ��VertexArray�ĸ߲����
/// �ڴ����жԲ��ֵ�buufer�������Եĸ�ֵ
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



