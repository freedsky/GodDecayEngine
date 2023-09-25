#include "gdpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace GodDecay 
{
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: GD_CLIENT_ASSERT(false, "RednererAPI is None"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLShader>(filepath);
		}

		GD_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (RendererAPI::GetAPI()) 
		{
			case RendererAPI::API::None: GD_CLIENT_ASSERT(false, "RednererAPI is None"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLShader>(vertexSrc, fragmentSrc);
		}

		GD_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
