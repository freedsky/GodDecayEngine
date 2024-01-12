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

		inline float& GetMetallic() { return m_Metallic; }
		inline void SetMetallic(float metallic) { m_Metallic = metallic; }
		inline float& GetRoughness() { return m_Roughness; }
		inline void SetRoughness(float roughness) { m_Roughness = roughness; }

		void LoadShader(std::string shaderName,std::string path);
		void LoadTexture(std::string mapName, TextureLibrary textures);

		//后续会在此函数中扩展不同种类的Shader，以及不同Shader下对应的Texture和Unifrom映射表
		void LoadShaderToRenderModel(std::string shaderName);
		//====================================
		void LoadTest();
		//BlinnPhong光照模型
		void BlinnPhongLight();
		//用于反射和折射的材质模型
		void RefectOrRefract();
		//Physical Base Render[PBR]光照渲染
		void PhysicalBaseRenderLight();
		//=====================================
	private:
		ShaderLibrary m_ShaderList;
		//建立映射表
		std::unordered_map<std::string, TextureLibrary> m_TextureList;
		std::unordered_map<std::string, UniformProperties> m_UniformProperties;
		//默认为白色
		glm::vec4 m_MeshColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		//PBR的材质信息[这部分都可以用纹理代替，这后面再说，现在用常量来代替]
		float m_Metallic;
		float m_Roughness;
		float m_Ao;
	};
}

