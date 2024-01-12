#pragma once

#include "Framebuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "GodDecay/Scene/Scene.h"

/// <summary>
/// 生成阴影贴图
/// 
/// 全局应当存在一份，为单例模式
/// </summary>

namespace GodDecay 
{
	class Shadow
	{
	public:
		~Shadow();
		//单例
		static Ref<Shadow> GetInstance();
		inline Ref<Framebuffer> GetShadowFramebuffer() { return m_ShadowFramebuffer; }
		inline glm::mat4& GetLightSpaceMatrix() { return LightSpaceMatrix; }
		inline void SetFramebufferWidth(uint32_t width) { m_FramebufferWidth = width; }
		inline uint32_t GetFramebufferWidth() { return m_FramebufferWidth; }
		inline void SetFramebufferHeight(uint32_t height) { m_FramebufferHeight = height; }
		inline uint32_t GetFramebufferHeight() { return m_FramebufferHeight; }
		inline void SetLightViewWidth(float width) { m_LightViewWidth = width; }
		inline float GetLightViewWidth() { return m_LightViewWidth; }

		//返回阴影贴图纹理
		inline Ref<Texture2D> GetShadowTexture() { return m_ShadowTexture; }

		//初始化数据[在修改数据时就会方便许多]
		void Init();

		////点光源阴影渲染
		//void DrawShadow(const glm::vec3& LightPosition, const Ref<Scene>& scene);
		//定向光源阴影渲染
		void DrawShadow(const glm::vec3& LightPosition, const Ref<Scene>& scene);

	private:
		Shadow();

		static Ref<Shadow> instance;

	private:
		//帧缓冲
		Ref<Framebuffer> m_ShadowFramebuffer;
		//Shader对象
		Ref<Shader> m_ShadowShader;
		//封装阴影纹理
		Ref<Texture2D> m_ShadowTexture;
		//帧的大小
		uint32_t m_FramebufferWidth;
		uint32_t m_FramebufferHeight;
		//用于转换为光空间下所需lookat矩阵变量
		float m_LightSpaceNear, m_LightSpaceFar;
		float m_LightViewWidth;
		glm::mat4 LightSpaceMatrix;
	public:
		//test
		uint32_t ShadowFarmebufferID;
		uint32_t ShadowTextureID;
	};
}

