#include "gdpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace GodDecay 
{
	Ref<RendererAPI> RenderCommand::m_RendererAPI = CreateRef<OpenGLRendererAPI>();
}
