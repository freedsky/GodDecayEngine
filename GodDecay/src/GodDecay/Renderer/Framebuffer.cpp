#include "gdpch.h"
#include "Framebuffer.h"

#include "GodDecay/Core/Base.h"

#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace GodDecay 
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: GD_ENGINE_ASSERT(false, "this UnKnow API"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLFramebuffer>(spec);
		}

		GD_ENGINE_ASSERT(false, "this UnKnow API");
		return nullptr;
	}
}
