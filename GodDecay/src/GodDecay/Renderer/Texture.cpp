#include "gdpch.h"
#include "Texture.h"

#include "GodDecay/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/OpenGL/OpenGLTextureCube.h"

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
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: GD_ENGINE_ASSERT(false, "this API is None"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture>(width, height);
		}

		GD_ENGINE_ASSERT(false, "UnKonw this API");
		return nullptr;
	}

	Ref<TextureCube> TextureCube::Create(std::vector<std::string> faces)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: GD_ENGINE_ASSERT(false, "this API is None"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTextureCube>(faces);
		}

		GD_ENGINE_ASSERT(false, "UnKonw this API");
		return nullptr;
	}

	Ref<TextureCube> TextureCube::Create(uint32_t width, uint32_t height)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: GD_ENGINE_ASSERT(false, "this API is None"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTextureCube>(width, height);
		}
	}
}
