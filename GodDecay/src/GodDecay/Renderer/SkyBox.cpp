#include "gdpch.h"
#include "SkyBox.h"

#include <glad/glad.h>

namespace GodDecay 
{
	//��պеĶ�������[��Ϊ����������ͼ���������ݾ���������������]
	
	//ת��Ϊ�ṹ��
	struct SkyBoxVertex 
	{
		glm::vec3 Position;
		int EntityID;
	};
	struct BrdfTextureVertex 
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		int EntityID;
	};


	float skyboxVertices[] = 
	{         
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	float brdfVertices[] = 
	{
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f
	};

	Ref<SkyBox> SkyBox::m_Instance = nullptr;

	SkyBox::SkyBox()
	{
		//��ʼ��Shader
		m_SkyBoxShader = Shader::Create("assets/shader/SkyBoxShader.glsl");
		m_HDRMapCubeShader = Shader::Create("assets/shader/HDRMapCube.glsl");
		m_IrradianceShader = Shader::Create("assets/shader/EnvironmentIrradiance.glsl");
		m_PrefilterTextureShader = Shader::Create("assets/shader/PrefilterTextureShader.glsl");
		m_BRDFLookUpTableShader = Shader::Create("assets/shader/BRDFLookUpTableShader.glsl");
		m_SkyBoxCubeTexture = nullptr;
		m_SkyBoxColorTexture = nullptr;
		m_CurrentSkyBoxTexture = nullptr;
		//HDR
		m_SkyBoxHDRFramebuffer = nullptr;
		m_HDRTexture = nullptr;
		m_SkyBoxCubeHDRTexture = nullptr;
		m_CurrentIsHDR = false;
	
		m_CurrentSkyType = SkyType::Normal;//��Ĭ��
		m_DispalySkyTextures.reserve(6);//ֱ��Ԥ��6���ռ��С

		m_SkyBoxColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		//׼����������
		m_SkyBoxBuffer = VertexArrayBuffer::Create();
		
		//�������м��ض�������
		std::vector<SkyBoxVertex> SkyBoxProperties;
		int arr_k = 0;
		for (int i = 0; i < 36; ++i) 
		{
			SkyBoxVertex sv;
			sv.Position.x = skyboxVertices[arr_k++];
			sv.Position.y = skyboxVertices[arr_k++];
			sv.Position.z = skyboxVertices[arr_k++];
			sv.EntityID = -1;

			SkyBoxProperties.push_back(sv);
		}

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(SkyBoxProperties.data(), SkyBoxProperties.size() * sizeof(SkyBoxVertex));
		vertexBuffer->Setlayout({
			{ShaderDataType::Float3, "a_Position" },
			{ShaderDataType::Int,    "a_EntityID"}
		});
		m_SkyBoxBuffer->AddVertexBuffer(vertexBuffer);
		SkyBoxProperties.clear();

		std::vector<uint32_t> m_SkyBoxIndices;
		for (uint32_t i = 0; i < 36; ++i)
		{
			m_SkyBoxIndices.push_back(i);
		}

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(m_SkyBoxIndices.data(), m_SkyBoxIndices.size());
		m_SkyBoxBuffer->SetIndexBuffer(indexBuffer);
		m_SkyBoxIndices.clear();

		//brdf���������Ķ��㻺��
		m_BRDFTextureBuffer = VertexArrayBuffer::Create();

		std::vector<BrdfTextureVertex> brdfProperties;
		arr_k = 0;
		for (int i = 0; i < 4; ++i)
		{
			BrdfTextureVertex sv;
			sv.Position.x = brdfVertices[arr_k++];
			sv.Position.y = brdfVertices[arr_k++];
			sv.Position.z = brdfVertices[arr_k++];
			sv.TexCoord.x = brdfVertices[arr_k++];
			sv.TexCoord.y = brdfVertices[arr_k++];
			sv.EntityID = -1;

			brdfProperties.push_back(sv);
		}

		Ref<VertexBuffer> bedfvBuffer = VertexBuffer::Create(brdfProperties.data(), brdfProperties.size() * sizeof(BrdfTextureVertex));
		bedfvBuffer->Setlayout({
			{ShaderDataType::Float3, "a_Position" },
			{ShaderDataType::Float2, "a_TexCoords"},
			{ShaderDataType::Int,    "a_EntityID"}
			});
		m_BRDFTextureBuffer->AddVertexBuffer(bedfvBuffer);
		brdfProperties.clear();

		//���ö��������±�
		std::vector<uint32_t> m_BrdfIndices;
		m_BrdfIndices.push_back(0);
		m_BrdfIndices.push_back(1);
		m_BrdfIndices.push_back(2);
		m_BrdfIndices.push_back(2);
		m_BrdfIndices.push_back(3);
		m_BrdfIndices.push_back(1);

		Ref<IndexBuffer> brefiBuffer = IndexBuffer::Create(m_BrdfIndices.data(), m_BrdfIndices.size());
		m_BRDFTextureBuffer->SetIndexBuffer(brefiBuffer);
		m_BrdfIndices.clear();

		//���ù���[ȫ�ֽ�һ��]//����������ڶ�mipmap�ͼ���μ���Խ��в�ֵ
		RenderCommand::SetCubeMapFiltration();
	}

	SkyBox::~SkyBox()
	{
	}

	Ref<SkyBox> SkyBox::GetInstance()
	{
		if (m_Instance == nullptr) 
		{
			//��Ref�ڹ���ʱ�ڲ���ֵ������ʹ���ⲿmake����ȥ������Ϊ��������private�Ĺ��캯��
			m_Instance = Ref<SkyBox>(new SkyBox());
		}
		return m_Instance;
	}

	//��ʼ����պ���ͼ���ߴ�ɫ
	void SkyBox::Init(std::vector<std::string> paths)
	{
		m_Paths = paths;
		//������ָ��һ��Ĭ�����  ��
		m_SkyBoxCubeTexture = TextureCube::Create(m_Paths);

		m_SkyBoxShader->Bind();
		m_SkyBoxShader->SetInt("skybox", 0);

		//����Ԥ��ͼ
		LoadDisplayTexture2D();
	}

	void SkyBox::Init(uint32_t width, uint32_t height)
	{
		//��shader������һ��Uniform��color���������ƴ�ɫ�ı仯
		m_SkyBoxColorTexture = TextureCube::Create(width, height, false);
		uint32_t whiteTextureData = 0xffffffff;
		m_SkyBoxColorTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		m_SkyBoxShader->Bind();
		m_SkyBoxShader->SetInt("skybox", 0);
	}
	//��ʼ��HDR���
	void SkyBox::Init(std::string path)
	{
		m_HDRPath = path;
		//����HDR����
		m_HDRTexture = Texture2D::Create(m_HDRPath, true);
		//����һ��HDR��Ⱦ����֡����
		FramebufferSpecification hdrSkyBoxSpec;
		hdrSkyBoxSpec.Attachments = { FramebufferRenderbufferFormat::Depth24 };
		hdrSkyBoxSpec.Width = 512.0f;
		hdrSkyBoxSpec.Height = 512.0f;
		m_SkyBoxHDRFramebuffer = Framebuffer::Create(hdrSkyBoxSpec);	
		//����һ��������Ⱦһ����������ͼ
		m_SkyBoxCubeHDRTexture = TextureCube::Create(512.0f, 512.0f, true);

		//Ȼ��Ӳ�ͬ�Ƕ�ȥ��Ⱦ����������
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViews[] =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};
		//��Ⱦ���������� �������±�
		m_HDRMapCubeShader->Bind();
		m_HDRMapCubeShader->SetInt("equirectangularMap", 0);
		
		//===========================================

		m_SkyBoxHDRFramebuffer->Bind();
		m_HDRMapCubeShader->Bind();
		for (uint32_t i = 0; i < 6; ++i)
		{
			//����viewproject
			glm::mat4 viewPro = captureProjection * captureViews[i];
			m_HDRMapCubeShader->SetMat4("u_ViewProjection", viewPro);
			m_SkyBoxHDRFramebuffer->AppendCubeTextureAttachment(m_SkyBoxCubeHDRTexture->GetRendererID(), i);
			RenderCommand::Clear();
			//��Ⱦ������
			/*
			* ԭ���������󶨳����ˡ�
			*/
			//��������
			m_HDRTexture->Bind(0);
			m_SkyBoxBuffer->Bind();
			RenderCommand::DrawIndexed(m_SkyBoxBuffer);
			m_SkyBoxBuffer->UnBind();
		}
		m_SkyBoxHDRFramebuffer->UnBind();

		//��������
		//���ն�����������ͼ
		m_IrradianceTextureCube = TextureCube::Create(64.0f, 64.0f, true);
		//��Ⱦ����ͼ��֡����
		FramebufferSpecification irradianceSpec;
		irradianceSpec.Attachments = { FramebufferRenderbufferFormat::Depth24 };
		irradianceSpec.Width = 64;
		irradianceSpec.Height = 64;
		m_IrradianceFramebuffer = Framebuffer::Create(irradianceSpec);

		//��ʼ��Ⱦ���ն�����ͼ
		m_IrradianceShader->Bind();
		m_IrradianceShader->SetInt("environmentMap", 0);

		m_IrradianceFramebuffer->Bind();
		for (uint32_t i = 0; i < 6; ++i) 
		{
			glm::mat4 viewPro = captureProjection * captureViews[i];
			m_IrradianceShader->SetMat4("u_ViewProjection", viewPro);
			m_IrradianceFramebuffer->AppendCubeTextureAttachment(m_IrradianceTextureCube->GetRendererID(), i);
			RenderCommand::Clear();

			m_SkyBoxCubeHDRTexture->Bind(0);
			m_SkyBoxBuffer->Bind();
			RenderCommand::DrawIndexed(m_SkyBoxBuffer);
			m_SkyBoxBuffer->UnBind();
		}
		m_IrradianceFramebuffer->UnBind();

		//��ʼ�������淴�䲿��
		//����һ������mipmap����������ͼ���˲�ͼ[����ͼ]
		//�����ڲ�ͬ�ֲڶ���������ͬ�㼶�Ĳ�ͬ�̶ȵľ���ͼ
		m_MipmapPrefilterTextureCube = TextureCube::Create(128.0f, 128.0f, true, true);
		//����һ����ȾԤ�������˲���������֡����
		FramebufferSpecification prefilterBoxSpec;
		prefilterBoxSpec.Attachments = { FramebufferRenderbufferFormat::Depth24 };
		prefilterBoxSpec.Width = 512.0f;
		prefilterBoxSpec.Height = 512.0f;
		m_PrefilterFramebuffer = Framebuffer::Create(prefilterBoxSpec);

		//��ȾԤ�˲�����
		m_PrefilterTextureShader->Bind();
		m_PrefilterTextureShader->SetInt("environmentMap", 0);

		m_PrefilterFramebuffer->Bind();
		uint32_t maxMipLevels = 5;//������ɵ�mipmap�����㼶
		for (uint32_t mip = 0; mip < maxMipLevels; ++mip) 
		{
			uint32_t mipWidth = 128 * std::pow(0.5, mip);
			uint32_t mipHeight = 128 * std::pow(0.5, mip);
			//�ڴ�֡�������ֻ��һ����Ⱦ���壬����±�Ϊ0
			m_PrefilterFramebuffer->AppendRenderbufferAttachment(mipWidth, mipHeight, m_PrefilterFramebuffer->GetDepthRenderbufferAttachmentRendererID(0), FramebufferRenderbufferFormat::Depth24);

			//��Ⱦ��ͬ�㼶����������ͼ
			float roughness = (float)mip / (float)(maxMipLevels - 1);
			m_PrefilterTextureShader->SetFloat("roughness", roughness);
			for (uint32_t i = 0; i < 6; ++i) 
			{
				glm::mat4 viewPro = captureProjection * captureViews[i];
				m_PrefilterTextureShader->SetMat4("u_ViewProjection", viewPro);
				m_PrefilterFramebuffer->AppendCubeTextureAttachment(m_MipmapPrefilterTextureCube->GetRendererID(), i, mip);
				RenderCommand::Clear();

				m_SkyBoxCubeHDRTexture->Bind(0);
				m_SkyBoxBuffer->Bind();
				RenderCommand::DrawIndexed(m_SkyBoxBuffer);
				m_SkyBoxBuffer->UnBind();
			}
		}
		m_PrefilterFramebuffer->UnBind();

		//����BRDF�Ļ��ֲ�������
		//����һ�ſ��Դ洢BRDF�Ĳ�������[2D]//Ҳ����˵���������þ���Ϊ�ղ���Ҫ�ض�����Ϊ��
		m_BRDFLookUpTableTexture = Texture2D::Create(512.0f, 512.0f, true);
		//����ȷʵ���Ը�����һ��ʹ�õ�֡���壬��������ѧϰĿ�ģ����ﻹ�����������Ա�ѧϰ�Ͳ鿴
		FramebufferSpecification brdfLookUpSpec;
		brdfLookUpSpec.Attachments = { FramebufferRenderbufferFormat::Depth24 };
		brdfLookUpSpec.Width = 512.0f;
		brdfLookUpSpec.Height = 512.0f;
		m_BRDFLookUpTableFramebuffer = Framebuffer::Create(brdfLookUpSpec);

		//��ȾBRDF���ֲ�������
		m_BRDFLookUpTableFramebuffer->Bind();
		m_BRDFLookUpTableFramebuffer->AppendRenderbufferAttachment(512.0f, 512.0f, m_BRDFLookUpTableFramebuffer->GetDepthRenderbufferAttachmentRendererID(0), FramebufferRenderbufferFormat::Depth24);
		m_BRDFLookUpTableFramebuffer->Append2DTextureAttachment(m_BRDFLookUpTableTexture->GetRendererID());
		m_BRDFLookUpTableShader->Bind();
		RenderCommand::Clear();
		//��Ⱦ����������
		m_BRDFTextureBuffer->Bind();
		RenderCommand::DrawIndexed(m_BRDFTextureBuffer);
		m_BRDFTextureBuffer->UnBind();

		m_BRDFLookUpTableFramebuffer->UnBind();

		//m_SkyBoxShader->Bind();
		//m_SkyBoxShader->SetInt("skybox", 0);
	}

	void SkyBox::LoadDisplayTexture2D()
	{
		//��0��5 ˳���ǣ�right.left.bottom.top.front.back
		if (!m_DispalySkyTextures.empty())
			m_DispalySkyTextures.clear();
		for (int i = 0; i < 6; ++i) 
		{
			Ref<Texture2D> texture = Texture2D::Create(m_Paths[i]);
			m_DispalySkyTextures.push_back(texture);
		}
	}

	void SkyBox::ChangeSkyBoxType(SkyType type)
	{
		//���ڲ���������CubeTexture��ת��ʱֻ��Ҫ�ѵ�ǰ��Tex�������ڲ���ֵ����
		//if (type == m_CurrentSkyType) return;
		switch (type)
		{
		case GodDecay::SkyBox::SkyType::Normal:
			m_CurrentSkyType = type;
			m_CurrentSkyBoxTexture = m_SkyBoxCubeTexture;
			m_CurrentIsHDR = false;
			break;
		case GodDecay::SkyBox::SkyType::PureColor:
			m_CurrentSkyType = type;
			m_CurrentSkyBoxTexture = m_SkyBoxColorTexture;
			m_CurrentIsHDR = false;
			break;
		case GodDecay::SkyBox::SkyType::HDR:
			m_CurrentSkyType = type;
			//m_CurrentSkyBoxTexture = m_MipmapPrefilterTextureCube;//���Կ��Կ���
			m_CurrentSkyBoxTexture = m_SkyBoxCubeHDRTexture;
			m_CurrentIsHDR = true;
			break;
		default:
			GD_ENGINE_WARN("Sorry Not Exit This Type Sky");
			break;
		}
	}

	//��Ⱦ��պ�
	void SkyBox::Update(const Camera& camera, const glm::mat4& transform)
	{
		//�Ȱ�transform��ƽ�������Ƴ�
		glm::mat4 view = glm::mat4(glm::mat3(transform));

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(view);
		//��պ���Զ����������������Ⱦ
		RenderCommand::SetDepthFunc(RendererAPI::DepthFunType::LEQUAL);
		m_SkyBoxShader->Bind();

		//����view�Լ�����uniform
		//��view�ĵ�����Ϊ�˰�ͶӰ�����е�ƽ�Ʋ��ָ��Ƴ����������ӽ����ƶ�ʱ����պп������ǲ����ģ���������Զ�ĸо�
		m_SkyBoxShader->SetMat4("u_ViewProjection", viewProj);
		m_SkyBoxShader->SetFloat4("skyColor", m_SkyBoxColor);

		m_CurrentSkyBoxTexture->Bind(0);

		m_SkyBoxBuffer->Bind();
		RenderCommand::DrawIndexed(m_SkyBoxBuffer);
		m_SkyBoxBuffer->UnBind();
		RenderCommand::SetDepthFunc(RendererAPI::DepthFunType::LESS);
	}

	void SkyBox::Update(const EditorCamera& camera)
	{
		//��պ���Զ����������������Ⱦ
		RenderCommand::SetDepthFunc(RendererAPI::DepthFunType::LEQUAL);
		m_SkyBoxShader->Bind();

		//����view�Լ�����uniform
		//��view�ĵ�����Ϊ�˰�ͶӰ�����е�ƽ�Ʋ��ָ��Ƴ����������ӽ����ƶ�ʱ����պп������ǲ����ģ���������Զ�ĸо�
		m_SkyBoxShader->SetMat4("u_ViewProjection", camera.GetSkyBoxViewProjection());
		m_SkyBoxShader->SetFloat4("skyColor", m_SkyBoxColor);
		m_SkyBoxShader->SetBool("IsHDR", m_CurrentIsHDR);

		m_CurrentSkyBoxTexture->Bind(0);

		m_SkyBoxBuffer->Bind();
		RenderCommand::DrawIndexed(m_SkyBoxBuffer);
		m_SkyBoxBuffer->UnBind();
		RenderCommand::SetDepthFunc(RendererAPI::DepthFunType::LESS);
	}
	
}