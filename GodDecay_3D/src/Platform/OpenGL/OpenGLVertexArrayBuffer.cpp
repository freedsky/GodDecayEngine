#include "gdpch.h"
#include "OpenGLVertexArrayBuffer.h"

#include <glad/glad.h>

namespace GodDecay 
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}

		GD_ENGINE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArrayBuffer::OpenGLVertexArrayBuffer()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArrayBuffer::~OpenGLVertexArrayBuffer()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArrayBuffer::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArrayBuffer::UnBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArrayBuffer::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		GD_ENGINE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "this Buffer layout is empty");

		Bind();
		vertexBuffer->Bind();

		uint32_t index = 0;
		const BufferLayout& layout = vertexBuffer->GetLayout();
		for (BufferElement element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)(intptr_t)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArrayBuffer::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		Bind();
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}
