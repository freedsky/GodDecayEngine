#pragma once

#include <glm/glm.hpp>
/// <summary>
/// 对Shader类进行与平台无关的的抽象
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

		//把常用的Uniform工具写成虚函数
		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name ,const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}



