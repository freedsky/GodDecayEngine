#pragma once
#include <GodDecayTop.h>

#include <imgui.h>

/// <summary>
/// ≤‚ ‘¿‡
/// </summary>
class GameLayer : public GodDecay::Layer
{
public:
	GameLayer(std::string name);
	~GameLayer();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpDate(float deltaTime) override;
	void OnEvents(GodDecay::Event& e) override;
	void OnImGuiRender() override;
private:
	GodDecay::Ref<GodDecay::OrthographicCameraController> m_Camera;
	GodDecay::Ref<GodDecay::Framebuffer> m_Framebuffer;

	glm::vec4 m_TextureColor;
	GodDecay::Ref<GodDecay::Texture2D> m_SquareTexture;
};

