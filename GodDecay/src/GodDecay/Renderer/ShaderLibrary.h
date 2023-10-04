#pragma once
#include <unordered_map>

#include "GodDecay/Core.h"
#include "Shader.h"

/// <summary>
/// Shader管理库用无序map集合进行管理
/// 当后期shader过多方便管理和调用
/// </summary>
namespace GodDecay 
{
	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);

		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);
		//这种方法可能在此后不会进行支持
		Ref<Shader> Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_ShaderLibraries;
	};
}



