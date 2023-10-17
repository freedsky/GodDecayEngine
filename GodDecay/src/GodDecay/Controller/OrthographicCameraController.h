#pragma once

#include "GodDecay/Core/Base.h"
#include "GodDecay/Tools/TimeTool.h"
#include "GodDecay/Renderer/OrthographicCamera.h"

#include "GodDecay/Events/ApplicationEvent.h"
#include "GodDecay/Events/MouseEvent.h"

/// <summary>
/// 2D���������
/// </summary>
namespace GodDecay 
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(float deltaTime);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		Ref<OrthographicCamera>& GetCamera() { return m_Camera; }
		const Ref<OrthographicCamera>& GetCamera() const { return m_Camera; }
		//���Ҫ��near��farƽ�����޸����ڴ�����ȥ��д���������������������SetOrthographicProjection�������и���
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:

		float m_AspectRatio;
		float m_ZoomLevel;

		bool m_Rotation;

		Ref<OrthographicCamera> m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;

	};
}


