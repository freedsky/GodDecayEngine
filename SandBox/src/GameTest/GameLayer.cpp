#include "GameLayer.h"

#include <glm/gtc/matrix_transform.hpp>

GameLayer::GameLayer(std::string name)
	:Layer(name),m_Camera(GodDecay::CreateRef<GodDecay::RendererCamera>(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 1.0f))
{
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
	float vertices[4 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.5f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
			 -0.5f,  0.5f, 0.0f, 0.1f, 0.1f, 0.5f, 1.0f
	};
	uint32_t indices[6] = { 0, 1, 2 ,2,3,0 };

	std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			out vec3 v_Position;
			out vec4 v_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_TransForm;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_TransForm * vec4(a_Position, 1.0);	
			}
		)";

	std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = v_Color;
			}
		)";

	m_Shader = GodDecay::Shader::Create(vertexSrc, fragmentSrc);

	vertexarray = GodDecay::VertexArrayBuffer::Create();


	GodDecay::Ref<GodDecay::VertexBuffer> vertex;


	vertex = GodDecay::VertexBuffer::Create(vertices, sizeof(vertices));

	GodDecay::BufferLayout layout =
	{
		{GodDecay::ShaderDataType::Float3,"a_Position"},
		{GodDecay::ShaderDataType::Float4,"a_Color"}
	};
	vertex->Setlayout(layout);

	vertexarray->AddVertexBuffer(vertex);

	GodDecay::Ref<GodDecay::IndexBuffer> index;
	index = GodDecay::IndexBuffer::Create(indices, sizeof(indices));
	vertexarray->SetIndexBuffer(index);
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpDate(float deltaTime)
{
	if (GodDecay::Input::IsKeyPressed(GODDECAY_KEY_A))
		m_CameraPosition.x -= Speeds * deltaTime;
	if (GodDecay::Input::IsKeyPressed(GODDECAY_KEY_D))
		m_CameraPosition.x += Speeds * deltaTime;
	if (GodDecay::Input::IsKeyPressed(GODDECAY_KEY_W))
		m_CameraPosition.y += Speeds * deltaTime;
	if (GodDecay::Input::IsKeyPressed(GODDECAY_KEY_S))
		m_CameraPosition.y -= Speeds * deltaTime;


	GodDecay::RenderCommand::SetClearColor(glm::vec4(0.1, 0.1, 0.1, 1.0));
	GodDecay::RenderCommand::Clear();

	m_Camera->Set2DPosition(m_CameraPosition);
	m_Camera->Set2DRotation(0.0f);

	GodDecay::Renderer::BeginScene(m_Camera);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			GodDecay::Renderer::Submit(m_Shader, vertexarray, transform);
		}
	}
}

void GameLayer::OnEvents(const GodDecay::Event& e)
{
	//GD_CLIENT_DEBUG("{0}", e.GetName());
}

void GameLayer::OnImGuiRender()
{
	ImGui::Begin("sda");
	ImGui::Text("sdas");
	ImGui::Button("new", ImVec2(100, 100));
	ImGui::End();
}
