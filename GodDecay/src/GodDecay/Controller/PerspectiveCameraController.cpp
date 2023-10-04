#include "gdpch.h"
#include "PerspectiveCameraController.h"

#include "GodDecay/Input.h"
#include "GodDecay/GodDecayKeyCodes.h"

#include "GodDecay/Application.h"

namespace GodDecay 
{
	bool firstMouse = true;
	bool hide_mouse = false;
	float Yaw = -90.0f, Pitch = 0.0f;

	PerspectiveCameraController::PerspectiveCameraController(float aspectRatio)
		:m_Camera(CreateRef<PerspectiveCamera>(45.0f, aspectRatio))
	{
		WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_CameraTranslationSpeed = 3.0f;
		m_CameraRotateSpeed = 0.1f;

		lastX = Application::Get().GetWindow()->GetWidth() / 2.0f;
		lastY = Application::Get().GetWindow()->GetHeight() / 2.0f;
	}

	void PerspectiveCameraController::OnUpdate(float deltaTime)
	{
		glm::vec3 front = m_Camera->GetPerspectiveFront();
		glm::vec3 right = m_Camera->GetPerspectiveRight();
		glm::vec3 position = m_Camera->GetPerspectivePosition();

		if (GodDecay::Input::IsKeyPressed(GODDECAY_KEY_W))
			position += front * (m_CameraTranslationSpeed * deltaTime);
		if (GodDecay::Input::IsKeyPressed(GODDECAY_KEY_S))
			position -= front * (m_CameraTranslationSpeed * deltaTime);
		if (GodDecay::Input::IsKeyPressed(GODDECAY_KEY_A))
			position -= right * (m_CameraTranslationSpeed * deltaTime);
		if (GodDecay::Input::IsKeyPressed(GODDECAY_KEY_D))
			position += right * (m_CameraTranslationSpeed * deltaTime);

		m_Camera->SetPerspectivePosition(position);
		m_Camera->UpDateViewMatrix(WorldUp);
		m_Camera->UpDateViewProjectionMatrix();

		if (GodDecay::Input::IsKeyPressed(GODDECAY_KEY_V))
		{
			if (!hide_mouse)
			{
				Application::Get().GetWindow()->SetHideMouse(!hide_mouse);
				hide_mouse = true;
			}
			else
			{
				Application::Get().GetWindow()->SetHideMouse(!hide_mouse);
				hide_mouse = false;
			}

		}
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&PerspectiveCameraController::OnMouseScrolled, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseMovedEvent>(std::bind(&PerspectiveCameraController::OnMosueMoved, this, std::placeholders::_1));
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		float offset = (float)e.GetYOffset();
		float zoom = m_Camera->GetZoomLevel();
		zoom -= offset;
		if (zoom < 1.0f)
			zoom = 1.0f;
		if (zoom > 45.0f)
			zoom = 45.0f;

		m_Camera->SetZoomLevel(zoom);
		m_Camera->UpDateProjectionMatrix();
		m_Camera->UpDateViewProjectionMatrix();
		return false;
	}
	bool PerspectiveCameraController::OnMosueMoved(MouseMovedEvent& e)
	{
		float xpos = static_cast<float>(e.GetX());
		float ypos = static_cast<float>(e.GetY());

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		xoffset *= m_CameraRotateSpeed;
		yoffset *= m_CameraRotateSpeed;

		Yaw += xoffset;
		Pitch += yoffset;

		if (true)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		m_Camera->SetPerspectiveFront(glm::normalize(front));

		m_Camera->UpDateViewMatrix(WorldUp);
		m_Camera->UpDateViewProjectionMatrix();

		return false;
	}
}
