#include "gdpch.h"
#include "Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer.h"

namespace GodDecay 
{
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::None: GD_ENGINE_ASSERT(false, "RednererAPI is None"); return nullptr;
			case RendererAPI::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}
		GD_ENGINE_ASSERT(false, "Plateform is not RendererAPI");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None: GD_ENGINE_ASSERT(false, "RednererAPI is None"); return nullptr;
			case RendererAPI::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, count);
		}
		GD_ENGINE_ASSERT(false, "Plateform is not RendererAPI");
		return nullptr;
	}
}
