#include "gdpch.h"
#include "Texture.h"

#include "GodDecay/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace GodDecay 
{
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (RendererAPI::GetAPI()) 
		{
		case RendererAPI::API::None: GD_ENGINE_ASSERT(false, "this API is None"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture>(path);
		}

		GD_ENGINE_ASSERT(false, "UnKonw this API"); 
		return nullptr;
	}
}
