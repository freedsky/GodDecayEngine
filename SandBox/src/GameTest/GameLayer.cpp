#include "GameLayer.h"

#include <glm/gtc/matrix_transform.hpp>

GameLayer::GameLayer(std::string name)
	:Layer(name), m_Camera(GodDecay::CreateRef<GodDecay::OrthographicCameraController>(1280.0f / 720.0f))
{
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
	float vertices[4 * 9] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f, 1.0, 0.0f,
			 0.5f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f, 1.0f, 1.0f,
			 -0.5f,  0.5f, 0.0f, 0.1f, 0.1f, 0.5f, 1.0f, 0.0f, 1.0f
	};
	uint32_t indices[6] = { 0, 1, 2 ,2,3,0 };
	m_ShaderLibraries = GodDecay::CreateRef<GodDecay::ShaderLibrary>();

	m_Shader = GodDecay::Shader::Create("assets/shader/Texture.glsl");

	m_Texture = GodDecay::Texture2D::Create("assets/texture/ChernoLogo.png");

	vertexarray = GodDecay::VertexArrayBuffer::Create();

	GodDecay::Ref<GodDecay::VertexBuffer> vertex;

	vertex = GodDecay::VertexBuffer::Create(vertices, sizeof(vertices));

	GodDecay::BufferLayout layout =
	{
		{GodDecay::ShaderDataType::Float3,"a_Position"},
		{GodDecay::ShaderDataType::Float4,"a_Color"},
		{GodDecay::ShaderDataType::Float2,"a_uv"}
	};
	vertex->Setlayout(layout);

	vertexarray->AddVertexBuffer(vertex);

	GodDecay::Ref<GodDecay::IndexBuffer> index;
	index = GodDecay::IndexBuffer::Create(indices, sizeof(indices));
	vertexarray->SetIndexBuffer(index);

	m_Shader->Bind();
	std::dynamic_pointer_cast<GodDecay::OpenGLShader>(m_Shader)->UploadUniformInt("u_Texture", 0);

	//加入到shader管理对象中
	m_ShaderLibraries->Add(m_Shader);
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpDate(float deltaTime)
{
	m_Camera->OnUpdate(deltaTime);

	GodDecay::RenderCommand::SetClearColor(glm::vec4(0.1, 0.1, 0.1, 1.0));
	GodDecay::RenderCommand::Clear();

	GodDecay::Renderer::BeginScene(m_Camera->GetCamera());
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			m_Texture->Bind(0);
			GodDecay::Renderer::Submit(m_Shader, vertexarray, transform);
		}
	}
}

void GameLayer::OnEvents(GodDecay::Event& e)
{
	m_Camera->OnEvent(e);
}

void GameLayer::OnImGuiRender()
{
	ImGui::Begin("sda");
	ImGui::Text("sdas");
	ImGui::Button("new", ImVec2(100, 100));
	ImGui::End();
}
