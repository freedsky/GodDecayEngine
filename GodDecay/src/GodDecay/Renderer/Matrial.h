#pragma once

#include "ShaderLibrary.h"
#include "TextureLibrary.h"
#include "UniformProperties.h"

/// <summary>
///	不同的Shader决定不同的物体材质
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

		//后续会在此函数中扩展不同种类的Shader，以及不同Shader下对应的Texture和Unifrom映射表
		void LoadShaderToRenderModel(std::string shaderName);
		//====================================
		void LoadTest();
		//BlinnPhong光照模型
		void BlinnPhongLight();

		//=====================================
	private:
		ShaderLibrary m_ShaderList;
		//建立映射表
		std::unordered_map<std::string, TextureLibrary> m_TextureList;
		std::unordered_map<std::string, UniformProperties> m_UniformProperties;
		//默认为白色
		glm::vec4 m_MeshColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		
	};
}

