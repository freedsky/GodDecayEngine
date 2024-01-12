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
		//初始化Shader
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
		//把帧缓冲进行初始化
		FramebufferSpecification shadowSpec;
		shadowSpec.Attachments = { FramebufferTextureFormat::Depth };
		shadowSpec.Width = m_FramebufferWidth;
		shadowSpec.Height = m_FramebufferHeight;
		m_ShadowFramebuffer = Framebuffer::Create(shadowSpec);
		m_ShadowTexture = Texture2D::Create("ShadowTexture", m_ShadowFramebuffer->GetDepthTextureAttachmentRendererID(), m_FramebufferWidth, m_FramebufferHeight);
	}

	//只有在有定向光的情况下才去计算渲染阴影贴图
	void Shadow::DrawShadow(const glm::vec3& LightPosition, const Ref<Scene>& scene)
	{
		m_ShadowFramebuffer->Bind();
		RenderCommand::ClearDepth();
		//先计算光角度下的观察矩阵
		glm::mat4 lightProjection = glm::ortho(-m_LightViewWidth, m_LightViewWidth, -m_LightViewWidth, m_LightViewWidth, m_LightSpaceNear, m_LightSpaceFar);
		//在外部判断
		glm::mat4 lightView = glm::lookAt(glm::vec3(LightPosition.x, -LightPosition.y, LightPosition.z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		LightSpaceMatrix = lightProjection * lightView;
		m_ShadowShader->Bind();
		m_ShadowShader->SetMat4("u_LightProjection", LightSpaceMatrix);

		//在这里也要对实体进行深度渲染//并未对物体进行深度渲染
		scene->OnUpdateDepth(m_ShadowShader);
		m_ShadowFramebuffer->UnBind();
	}
}
