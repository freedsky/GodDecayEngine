#include "gdpch.h"
#include "OrthographicCameraController.h"

#include "GodDecay/Core/Input.h"
#include "GodDecay/Core/GodDecayKeyCodes.h"

namespace GodDecay 
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Rotation(rotation)
	{
		m_ZoomLevel = 1.0f;
		m_Camera = CreateRef<OrthographicCamera>(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1, 1);

	}

	void OrthographicCameraController::OnUpdate(float deltaTime)
	{
		if (GodDecay::Input::IsKeyPressed(GODDECAY_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * deltaTime;
		if (GodDecay::Input::IsKeyPressed(GODDECAY_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * deltaTime;
		if (GodDecay::Input::IsKeyPressed(GODDECAY_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * deltaTime;
		if (GodDecay::Input::IsKeyPressed(GODDECAY_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * deltaTime;

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(GODDECAY_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * deltaTime;
			if (Input::IsKeyPressed(GODDECAY_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * deltaTime;

			m_Camera->SetRotation(m_CameraRotation);
		}

		m_Camera->SetPosition(m_CameraPosition);


		m_CameraRotationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&OrthographicCameraController::OnMouseScrolled, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&OrthographicCameraController::OnWindowResized, this, std::placeholders::_1));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera->SetOrthographicProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, m_Camera->GetNearPlan(), m_Camera->GetFarPlan());
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera->SetOrthographicProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, m_Camera->GetNearPlan(), m_Camera->GetFarPlan());
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}
