#include "gdpch.h"
#include "ImGuiLayer.h"

#include <GLFW/glfw3.h>
#include <imgui.h>

#include "Platform/OpenGL/OpenGLImGui/imgui_impl_opengl3.h"
#include "GodDecay/Application.h"

namespace GodDecay 
{
	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void ImGuiLayer::OnDetach()
	{
		
	}

	void ImGuiLayer::OnUpDate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow()->GetWidth(), app.GetWindow()->GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	//ImGui���Լ����¼�ʵ�֣����������ʱ�ر�
	void ImGuiLayer::OnEvents(const Event& e)
	{
		//GD_ENGINE_DEBUG("{0}", e);
	}
}


