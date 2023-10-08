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
	GodDecay::Ref<GodDecay::PerspectiveCameraController> m_Camera;

	glm::vec3 m_CameraPosition = glm::vec3(0.0f);
	float Speeds = 1.0f;

	glm::vec4 m_2DSquareColor;
	glm::vec4 m_3DCubeColor;
	glm::vec4 m_3DCirleColor;

	GodDecay::Ref<GodDecay::Model> model;
};

