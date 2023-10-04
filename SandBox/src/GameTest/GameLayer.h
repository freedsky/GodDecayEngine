#pragma once
#include <GodDecay/Layer.h>
#include <GodDecay/Logger.h>
#include <GodDecay/Input.h>

#include "GodDecay/Renderer/Buffer.h"
#include "GodDecay/Renderer/VertexArrayBuffer.h"
#include "GodDecay/Renderer/RenderCommand.h"
#include "GodDecay/Renderer/Renderer.h"
#include "GodDecay/Controller/OrthographicCameraController.h"
#include "GodDecay/Controller/PerspectiveCameraController.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "GodDecay/Renderer/ShaderLibrary.h"
#include "GodDecay/Renderer/Texture.h"

#include "GodDecay/GodDecayKeyCodes.h"
#include "GodDecay/GodDecayMouseButtonCodes.h"

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
	GodDecay::Ref< GodDecay::ShaderLibrary> m_ShaderLibraries;

	GodDecay::Ref<GodDecay::Shader> m_Shader;

	GodDecay::Ref<GodDecay::VertexArrayBuffer> vertexarray;

	GodDecay::Ref<GodDecay::OrthographicCameraController> m_OrthographicCamera;
	GodDecay::Ref<GodDecay::PerspectiveCameraController> m_PerspectiveCamera;

	glm::vec3 m_CameraPosition = glm::vec3(0.0f);
	float Speeds = 1.0f;

	GodDecay::Ref<GodDecay::Texture2D> m_Texture;
};

