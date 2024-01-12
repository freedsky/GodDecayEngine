#pragma once

#include "ShaderLibrary.h"
#include "TextureLibrary.h"
#include "UniformProperties.h"

/// <summary>
///	��ͬ��Shader������ͬ���������
/// </summary>
namespace GodDecay 
{
	class Matrial
	{
	public:
		Matrial();
		~Matrial() = default;

		inline ShaderLibrary& GetShaderList() { return m_ShaderList; }
		inline TextureLibrary& GetTextureList(std::string shaderName) { return m_TextureList[shaderName]; }
		inline glm::vec4& GetMeshColor() { return m_MeshColor; }
		inline UniformProperties& GetUniformProperties(std::string shaderName) { return m_UniformProperties[shaderName]; }

		inline float& GetMetallic() { return m_Metallic; }
		inline void SetMetallic(float metallic) { m_Metallic = metallic; }
		inline float& GetRoughness() { return m_Roughness; }
		inline void SetRoughness(float roughness) { m_Roughness = roughness; }

		void LoadShader(std::string shaderName,std::string path);
		void LoadTexture(std::string mapName, TextureLibrary textures);

		//�������ڴ˺�������չ��ͬ�����Shader���Լ���ͬShader�¶�Ӧ��Texture��Unifromӳ���
		void LoadShaderToRenderModel(std::string shaderName);
		//====================================
		void LoadTest();
		//BlinnPhong����ģ��
		void BlinnPhongLight();
		//���ڷ��������Ĳ���ģ��
		void RefectOrRefract();
		//Physical Base Render[PBR]������Ⱦ
		void PhysicalBaseRenderLight();
		//=====================================
	private:
		ShaderLibrary m_ShaderList;
		//����ӳ���
		std::unordered_map<std::string, TextureLibrary> m_TextureList;
		std::unordered_map<std::string, UniformProperties> m_UniformProperties;
		//Ĭ��Ϊ��ɫ
		glm::vec4 m_MeshColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		//PBR�Ĳ�����Ϣ[�ⲿ�ֶ�������������棬�������˵�������ó���������]
		float m_Metallic;
		float m_Roughness;
		float m_Ao;
	};
}

