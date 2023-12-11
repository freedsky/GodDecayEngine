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

		void LoadShader(std::string shaderName,std::string path);
		void LoadTexture(std::string mapName, TextureLibrary textures);

		//�������ڴ˺�������չ��ͬ�����Shader���Լ���ͬShader�¶�Ӧ��Texture��Unifromӳ���
		void LoadShaderToRenderModel(std::string shaderName);
		//====================================
		void LoadTest();
		//BlinnPhong����ģ��
		void BlinnPhongLight();

		//=====================================
	private:
		ShaderLibrary m_ShaderList;
		//����ӳ���
		std::unordered_map<std::string, TextureLibrary> m_TextureList;
		std::unordered_map<std::string, UniformProperties> m_UniformProperties;
		//Ĭ��Ϊ��ɫ
		glm::vec4 m_MeshColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		
	};
}

