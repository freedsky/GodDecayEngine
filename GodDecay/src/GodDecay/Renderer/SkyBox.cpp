#include "gdpch.h"
#include "SkyBox.h"

namespace GodDecay 
{
	//��պеĶ�������[��Ϊ����������ͼ���������ݾ���������������]
	
	//ת��Ϊ�ṹ��
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
		//��ʼ��Shader
		m_SkyBoxShader = Shader::Create("assets/shader/SkyBoxShader.glsl");
		m_SkyBoxCubeTexture = nullptr;
		m_SkyBoxColorTexture = nullptr;
		m_CurrentSkyBoxTexture = nullptr;
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
		LoadDispalyTexture2D();
	}

	void SkyBox::Init(uint32_t width, uint32_t height)
	{
		//��shader�����һ��Uniform��color���������ƴ�ɫ�ı仯
		m_SkyBoxColorTexture = TextureCube::Create(width, height);
		uint32_t whiteTextureData = 0xffffffff;
		m_SkyBoxColorTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		m_SkyBoxShader->Bind();
		m_SkyBoxShader->SetInt("skybox", 0);
	}

	void SkyBox::LoadDispalyTexture2D()
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
			break;
		case GodDecay::SkyBox::SkyType::PureColor:
			m_CurrentSkyType = type;
			m_CurrentSkyBoxTexture = m_SkyBoxColorTexture;
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

		m_CurrentSkyBoxTexture->Bind(0);

		m_SkyBoxBuffer->Bind();
		RenderCommand::DrawIndexed(m_SkyBoxBuffer);
		m_SkyBoxBuffer->UnBind();
		RenderCommand::SetDepthFunc(RendererAPI::DepthFunType::LESS);
	}
	
}
