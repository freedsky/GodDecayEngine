#pragma once

#include "Camera.h"
#include "GodDecay/Events/Event.h"
#include "GodDecay/Events/MouseEvent.h"

#include <glm/glm.hpp>

/// <summary>
/// 编辑相机，在Scene中提供可以检视的相机
/// 与实际的运行时相机有所不同，它只存在与编辑界面当中
/// </summary>
namespace GodDecay
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(float deltaTime);
		void OnEvent(Event& e);

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distacne) { m_Distance = distacne; }
		//调整相机的视口大小
		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }
		
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }
		//天空盒视图投影矩阵
		glm::mat4& GetSkyBoxViewProjection() const;

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		glm::vec3 GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }
	private:
		//更新view矩阵和projective矩阵
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		//用鼠标控制相机的旋转和zoom的缩放
		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		//控制相机的各种速度
		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

	private:
		//相机的基本参数
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		//相机的基本矩阵
		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = { 0.0f,0.0f,0.0f };
		glm::vec3 m_FocalPoint = { 0.0f,0.0f,0.0f };

		//鼠标位置
		glm::vec2 m_InitialMousePosition = { 0.0f,0.0f };

		//控制旋转变量
		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		float m_ViewportWidth = 1280.0f, m_ViewportHeight = 720.0f;
	};
}



