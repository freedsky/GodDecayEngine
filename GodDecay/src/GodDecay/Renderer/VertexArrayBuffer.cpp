#include "gdpch.h"
#include "VertexArrayBuffer.h"

#include "GodDecay/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArrayBuffer.h"

namespace GodDecay 
{
	Ref<VertexArrayBuffer> VertexArrayBuffer::Create()
	{
		
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None: GD_ENGINE_ASSERT(false, "RednererAPI is None"); return nullptr;
			case RendererAPI::OpenGL: return CreateRef<OpenGLVertexArrayBuffer>();
		}

		GD_ENGINE_ASSERT(false, "Plateform is not RendererAPI");
		return nullptr;
	}
}
