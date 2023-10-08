#pragma once

#include <glm/glm.hpp>
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

		//�ѳ��õ�Uniform����д���麯��
		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name ,const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}



