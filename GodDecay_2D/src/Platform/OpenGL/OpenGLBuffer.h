#pragma once

#include "GodDecay/Renderer/Buffer.h"
/// <summary>
/// 在OpenGL API下的对buffer对实现
/// </summary>
namespace GodDecay 
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_Bufferlayout; }
		virtual void Setlayout(const BufferLayout& layout) override { m_Bufferlayout = layout; }
	private:
		uint32_t m_VertexRendererID;
		BufferLayout m_Bufferlayout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_IndexRendererID;
		uint32_t m_Count;
	};
}



