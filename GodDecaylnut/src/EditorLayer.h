#pragma once
#include <GodDecayTop.h>

#include <imgui.h>

/// <summary>
/// ÒýÇæ±à¼­²ã
/// </summary>
 
namespace GodDecay 
{
	class EditorLayer : public Layer
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
		Ref<GodDecay::PerspectiveCameraController> m_Camera;
		GodDecay::Ref<GodDecay::Framebuffer> m_Framebuffer;

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		//-------TEST-------------------------
		glm::vec4 m_2DSquareColor;
		glm::vec4 m_3DCubeColor;
		glm::vec4 m_3DCirleColor;
		Ref<Texture2D> m_SqueraTexure;
		Ref<Texture2D> m_CubeTexture;
		Ref<Texture2D> m_CirleTexture;
		Ref<Texture2D> m_ModelTexture;
	};
}