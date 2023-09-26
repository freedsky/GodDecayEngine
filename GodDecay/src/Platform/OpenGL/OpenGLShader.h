#pragma once

#include "GodDecay/Renderer/Shader.h"

#include <glm/glm.hpp>

/// <summary>
/// ��OpenGl��ʵ��Shader����
/// �����˶�shader�ļ����ⲿ��д����[��vert��frag����ͬһ�ļ���ȥ����]
/// </summary>
namespace GodDecay 
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& filepath);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual const std::string& GetShaderName() const override { return m_ShaderName; }

		std::string ReadFile(const std::string& filepath);
		std::unordered_map<uint32_t, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<uint32_t, std::string>& shaderSources);

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformBool(const std::string& name, const bool& value);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		uint32_t m_RendererID;
		std::string m_ShaderName;
	};
}



