#include "gdpch.h"
#include "ShaderLibrary.h"

namespace GodDecay 
{
	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		GD_ENGINE_ASSERT(!Exists(name), "Shader already exists!");
		m_ShaderLibraries[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		std::string name = shader->GetShaderName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		Ref<Shader> shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		Ref<Shader> shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		Ref<Shader> shader = Shader::Create(name, vertexSrc, fragmentSrc);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		GD_ENGINE_ASSERT(Exists(name), "Shader not found!");
		return m_ShaderLibraries[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_ShaderLibraries.find(name) != m_ShaderLibraries.end();
	}
}
