#pragma once

#include "GodDecay/Core/Base.h"
#include "GodDecay/Tools/TimeTool.h"
#include "GodDecay/Renderer/OrthographicCamera.h"

#include "GodDecay/Events/ApplicationEvent.h"
#include "GodDecay/Events/MouseEvent.h"

/// <summary>
/// 2D相机控制类
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
		//如果要对near和far平面做修改请在此类中去编写函数，并在最后调用相机的SetOrthographicProjection方法进行更新
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


