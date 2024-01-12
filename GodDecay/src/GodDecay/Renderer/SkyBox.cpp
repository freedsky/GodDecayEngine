#include "gdpch.h"
#include "SkyBox.h"

#include <glad/glad.h>

namespace GodDecay 
{
	//天空盒的顶点数据[因为是立方体贴图，顶点数据就是它的纹理坐标]
	
	//转换为结构体
	struct SkyBoxVertex 
	{
		glm::vec3 Position;
		int EntityID;
	};


	float skyboxVertices[] = {         
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

	Ref<SkyBox> SkyBox::m_Instance = nullptr;

	SkyBox::SkyBox()
	{
		//初始化Shader
		m_SkyBoxShader = Shader::Create("assets/shader/SkyBoxShader.glsl");
		m_HDRMapCubeShader = Shader::Create("assets/shader/HDRMapCube.glsl");
		m_IrradianceShader = Shader::Create("assets/shader/EnvironmentIrradiance.glsl");
		m_SkyBoxCubeTexture = nullptr;
		m_SkyBoxColorTexture = nullptr;
		m_CurrentSkyBoxTexture = nullptr;
		//HDR
		m_SkyBoxHDRFramebuffer = nullptr;
		m_HDRTexture = nullptr;
		m_SkyBoxCubeHDRTexture = nullptr;
		m_CurrentIsHDR = false;
	
		m_CurrentSkyType = SkyType::Normal;//先默认
		m_DispalySkyTextures.reserve(6);//直接预留6个空间大小

		m_SkyBoxColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		//准备顶点数据
		m_SkyBoxBuffer = VertexArrayBuffer::Create();
		
		//从数组中加载顶点属性
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
	}

	SkyBox::~SkyBox()
	{
	}

	Ref<SkyBox> SkyBox::GetInstance()
	{
		if (m_Instance == nullptr) 
		{
			//让Ref在构造时内部赋值，而不使用外部make函数去构造因为单例类是private的构造函数
			m_Instance = Ref<SkyBox>(new SkyBox());
		}
		return m_Instance;
	}

	//初始化天空盒贴图或者纯色
	void SkyBox::Init(std::vector<std::string> paths)
	{
		m_Paths = paths;
		//可以先指定一个默认天空  盒
		m_SkyBoxCubeTexture = TextureCube::Create(m_Paths);

		m_SkyBoxShader->Bind();
		m_SkyBoxShader->SetInt("skybox", 0);

		//加载预览图
		LoadDisplayTexture2D();
	}

	void SkyBox::Init(uint32_t width, uint32_t height)
	{
		//在shader中添加一个Uniform的color变量来控制纯色的变化
		m_SkyBoxColorTexture = TextureCube::Create(width, height, false);
		uint32_t whiteTextureData = 0xffffffff;
		m_SkyBoxColorTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		m_SkyBoxShader->Bind();
		m_SkyBoxShader->SetInt("skybox", 0);
	}
	//初始化HDR天空
	void SkyBox::Init(std::string path)
	{
		m_HDRPath = path;
		//加载HDR纹理
		m_HDRTexture = Texture2D::Create(m_HDRPath, true);
		//创建一个HDR渲染缓冲帧对象
		FramebufferSpecification hdrSkyBoxSpec;
		hdrSkyBoxSpec.Attachments = { FramebufferRenderbufferFormat::Depth24 };
		hdrSkyBoxSpec.Width = 1024.0f;
		hdrSkyBoxSpec.Height = 1024.0f;
		m_SkyBoxHDRFramebuffer = Framebuffer::Create(hdrSkyBoxSpec);	
		//创建一个用于渲染一个立方体贴图
		m_SkyBoxCubeHDRTexture = TextureCube::Create(1024.0f, 1024.0f, true);

		//然后从不同角度去渲染立方体纹理
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
		//渲染立方体纹理 绑定纹理下标
		m_HDRMapCubeShader->Bind();
		m_HDRMapCubeShader->SetInt("equirectangularMap", 0);
		
		//===========================================

		m_SkyBoxHDRFramebuffer->Bind();
		m_HDRMapCubeShader->Bind();
		for (uint32_t i = 0; i < 6; ++i)
		{
			//计算viewproject
			glm::mat4 viewPro = captureProjection * captureViews[i];
			m_HDRMapCubeShader->SetMat4("u_ViewProjection", viewPro);
			m_SkyBoxHDRFramebuffer->AppendCubeTextureAttachment(m_SkyBoxCubeHDRTexture->GetRendererID(), i);
			RenderCommand::Clear();
			//渲染立方体
			/*
			* 原来是纹理绑定出错了。
			*/
			//激活纹理
			m_HDRTexture->Bind(0);
			m_SkyBoxBuffer->Bind();
			RenderCommand::DrawIndexed(m_SkyBoxBuffer);
			m_SkyBoxBuffer->UnBind();
		}
		m_SkyBoxHDRFramebuffer->UnBind();

		//卷积部分
		//辐照度立方体纹理图
		m_IrradianceTextureCube = TextureCube::Create(32.0f, 32.0f, true);
		//渲染辐照图的帧缓冲
		FramebufferSpecification irradianceSpec;
		irradianceSpec.Attachments = { FramebufferRenderbufferFormat::Depth24 };
		irradianceSpec.Width = 32;
		irradianceSpec.Height = 32;
		m_IrradianceFramebuffer = Framebuffer::Create(irradianceSpec);

		//开始渲染辐照度纹理图
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


		m_SkyBoxShader->Bind();
		m_SkyBoxShader->SetInt("skybox", 0);
	}

	void SkyBox::LoadDisplayTexture2D()
	{
		//从0到5 顺序是：right.left.bottom.top.front.back
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
		//在内部建立两个CubeTexture，转换时只需要把当前的Tex重新在内部赋值即可
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
			//m_CurrentSkyBoxTexture = m_IrradianceTextureCube;测试可以开启
			m_CurrentSkyBoxTexture = m_SkyBoxCubeHDRTexture;
			m_CurrentIsHDR = true;
			break;
		default:
			GD_ENGINE_WARN("Sorry Not Exit This Type Sky");
			break;
		}
	}

	//渲染天空盒
	void SkyBox::Update(const Camera& camera, const glm::mat4& transform)
	{
		//先把transform的平移属性移除
		glm::mat4 view = glm::mat4(glm::mat3(transform));

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(view);
		//天空盒永远先于其他物体先渲染
		RenderCommand::SetDepthFunc(RendererAPI::DepthFunType::LEQUAL);
		m_SkyBoxShader->Bind();

		//调整view以及更新uniform
		//对view的调整是为了把投影矩阵中的平移部分给移除，这样在视角在移动时，天空盒看起来是不动的，给人无限远的感觉
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
		//天空盒永远先于其他物体先渲染
		RenderCommand::SetDepthFunc(RendererAPI::DepthFunType::LEQUAL);
		m_SkyBoxShader->Bind();

		//调整view以及更新uniform
		//对view的调整是为了把投影矩阵中的平移部分给移除，这样在视角在移动时，天空盒看起来是不动的，给人无限远的感觉
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
