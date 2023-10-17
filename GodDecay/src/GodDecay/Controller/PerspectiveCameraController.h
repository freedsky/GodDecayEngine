#pragma once

#include "GodDecay/Core/Base.h"
#include "GodDecay/Tools/TimeTool.h"
#include "GodDecay/Renderer/PerspectiveCamera.h"
		  
#include "GodDecay/Events/ApplicationEvent.h"
#include "GodDecay/Events/MouseEvent.h"
#include "GodDecay/Events/KeyEvent.h"

/// <summary>
/// 3D���������
/// </summary>
namespace GodDecay 
{
	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float aspectRatio);
		~PerspectiveCameraController() = default;

		void OnUpdate(float deltaTime);
		void OnEvent(Event & e);

		void OnResize(float width, float height);

		Ref<PerspectiveCamera>& GetCamera() { return m_Camera; }
		const Ref<PerspectiveCamera>& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent & e);
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnMosueMoved(MouseMovedEvent & e);
		bool OnKeyDown(KeyPressedEvent& e);
	private:
		glm::vec3 WorldUp;
		float m_CameraTranslationSpeed;
		float m_CameraRotateSpeed;

		float lastX;
		float lastY;


		Ref<PerspectiveCamera> m_Camera;
	};
}



