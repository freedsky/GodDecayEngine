#pragma once
#include <unordered_map>

#include "GodDecay/Core.h"
#include "Shader.h"

/// <summary>
/// Shader�����������map���Ͻ��й���
/// ������shader���෽�����͵���
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
		//���ַ��������ڴ˺󲻻����֧��
		Ref<Shader> Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_ShaderLibraries;
	};
}



