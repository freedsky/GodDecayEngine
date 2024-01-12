#pragma once

#include "Framebuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "GodDecay/Scene/Scene.h"

/// <summary>
/// ������Ӱ��ͼ
/// 
/// ȫ��Ӧ������һ�ݣ�Ϊ����ģʽ
/// </summary>

namespace GodDecay 
{
	class Shadow
	{
	public:
		~Shadow();
		//����
		static Ref<Shadow> GetInstance();
		inline Ref<Framebuffer> GetShadowFramebuffer() { return m_ShadowFramebuffer; }
		inline glm::mat4& GetLightSpaceMatrix() { return LightSpaceMatrix; }
		inline void SetFramebufferWidth(uint32_t width) { m_FramebufferWidth = width; }
		inline uint32_t GetFramebufferWidth() { return m_FramebufferWidth; }
		inline void SetFramebufferHeight(uint32_t height) { m_FramebufferHeight = height; }
		inline uint32_t GetFramebufferHeight() { return m_FramebufferHeight; }
		inline void SetLightViewWidth(float width) { m_LightViewWidth = width; }
		inline float GetLightViewWidth() { return m_LightViewWidth; }

		//������Ӱ��ͼ����
		inline Ref<Texture2D> GetShadowTexture() { return m_ShadowTexture; }

		//��ʼ������[���޸�����ʱ�ͻ᷽�����]
		void Init();

		////���Դ��Ӱ��Ⱦ
		//void DrawShadow(const glm::vec3& LightPosition, const Ref<Scene>& scene);
		//�����Դ��Ӱ��Ⱦ
		void DrawShadow(const glm::vec3& LightPosition, const Ref<Scene>& scene);

	private:
		Shadow();

		static Ref<Shadow> instance;

	private:
		//֡����
		Ref<Framebuffer> m_ShadowFramebuffer;
		//Shader����
		Ref<Shader> m_ShadowShader;
		//��װ��Ӱ����
		Ref<Texture2D> m_ShadowTexture;
		//֡�Ĵ�С
		uint32_t m_FramebufferWidth;
		uint32_t m_FramebufferHeight;
		//����ת��Ϊ��ռ�������lookat�������
		float m_LightSpaceNear, m_LightSpaceFar;
		float m_LightViewWidth;
		glm::mat4 LightSpaceMatrix;
	public:
		//test
		uint32_t ShadowFarmebufferID;
		uint32_t ShadowTextureID;
	};
}

