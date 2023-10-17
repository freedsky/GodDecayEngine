#pragma once

#include "GodDecay/Renderer/Buffer.h"
#include "GodDecay/Renderer/VertexArrayBuffer.h"

namespace GodDecay 
{
	class OpenGLVertexArrayBuffer : public VertexArrayBuffer
	{
	public:
		OpenGLVertexArrayBuffer();
		virtual ~OpenGLVertexArrayBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual std::vector<Ref<VertexBuffer>> GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual Ref<IndexBuffer> GetIndexBuffer() const override { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}



