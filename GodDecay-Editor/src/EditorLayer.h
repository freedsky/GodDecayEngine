#pragma once
#include <GodDecayTop.h>

#include <imgui.h>

/// <summary>
/// ≤‚ ‘¿‡
/// </summary>

namespace GodDecay 
{
	class EditorLayer : public GodDecay::Layer
	{
	public:
		EditorLayer(std::string name);
		~EditorLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpDate(float deltaTime) override;
		void OnEvents(Event& e) override;
		void OnImGuiRender() override;
	private:
		Ref<OrthographicCameraController> m_Camera;
		GodDecay::Ref<GodDecay::Framebuffer> m_Framebuffer;

		glm::vec4 m_TextureColor;
		Ref<Texture2D> m_SquareTexture;
	};
}

