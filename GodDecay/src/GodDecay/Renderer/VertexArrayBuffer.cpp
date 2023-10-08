#include "gdpch.h"
#include "VertexArrayBuffer.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLVertexArrayBuffer.h"

namespace GodDecay 
{
	Ref<VertexArrayBuffer> VertexArrayBuffer::Create()
	{
		
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: GD_ENGINE_ASSERT(false, "RednererAPI is None"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexArrayBuffer>();
		}

		GD_ENGINE_ASSERT(false, "Plateform is not RendererAPI");
		return nullptr;
	}
}
