#include "gdpch.h"
#include "Shadow.h"

#include "SceneLightController.h"
#include "RenderCommand.h"
#include "GodDecay/Scene/Scene.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace GodDecay 
{
	Ref<Shadow> Shadow::instance = nullptr;

	Shadow::Shadow()
		:m_FramebufferWidth(1024), m_FramebufferHeight(1024),
		m_LightSpaceNear(1.0f), m_LightSpaceFar(7.5f), m_LightViewWidth(10.0f)
	{
		//��ʼ��Shader
		m_ShadowShader = Shader::Create("assets/shader/ShadowShader.glsl");
		m_ShadowTexture = nullptr;
	}
	Shadow::~Shadow()
	{
	}

	Ref<Shadow> Shadow::GetInstance()
	{
		if (instance == nullptr) 
		{
			instance = Ref<Shadow>(new Shadow());
		}
		return instance;
	}

	void Shadow::Init()
	{
		//��֡������г�ʼ��
		FramebufferSpecification shadowSpec;
		shadowSpec.Attachments = { FramebufferTextureFormat::Depth };
		shadowSpec.Width = m_FramebufferWidth;
		shadowSpec.Height = m_FramebufferHeight;
		m_ShadowFramebuffer = Framebuffer::Create(shadowSpec);
		m_ShadowTexture = Texture2D::Create("ShadowTexture", m_ShadowFramebuffer->GetDepthTextureAttachmentRendererID(), m_FramebufferWidth, m_FramebufferHeight);
	}

	//ֻ�����ж���������²�ȥ������Ⱦ��Ӱ��ͼ
	void Shadow::DrawShadow(const glm::vec3& LightPosition, const Ref<Scene>& scene)
	{
		m_ShadowFramebuffer->Bind();
		RenderCommand::ClearDepth();
		//�ȼ����Ƕ��µĹ۲����
		glm::mat4 lightProjection = glm::ortho(-m_LightViewWidth, m_LightViewWidth, -m_LightViewWidth, m_LightViewWidth, m_LightSpaceNear, m_LightSpaceFar);
		//���ⲿ�ж�
		glm::mat4 lightView = glm::lookAt(glm::vec3(LightPosition.x, -LightPosition.y, LightPosition.z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		LightSpaceMatrix = lightProjection * lightView;
		m_ShadowShader->Bind();
		m_ShadowShader->SetMat4("u_LightProjection", LightSpaceMatrix);

		//������ҲҪ��ʵ����������Ⱦ//��δ��������������Ⱦ
		scene->OnUpdateDepth(m_ShadowShader);
		m_ShadowFramebuffer->UnBind();
	}
}
