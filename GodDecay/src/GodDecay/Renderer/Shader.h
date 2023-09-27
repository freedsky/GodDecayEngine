#pragma once

/// <summary>
/// ��Shader�������ƽ̨�޹صĵĳ���
/// </summary>
namespace GodDecay 
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual const std::string& GetShaderName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name ,const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}


