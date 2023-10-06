#pragma once

#include "GodDecay/Core.h"

#include <glm/glm.hpp>


/// <summary>
/// 3D相机[FPS风格]
/// </summary>
namespace GodDecay 
{
	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(float zoom, float aspect, float n = 0.01f, float f = 100.0f);
		~PerspectiveCamera() = default;

		inline void SetPerspectivePosition(const glm::vec3 & position) { PerspectivePosition = position; }
		inline glm::vec3& GetPerspectivePosition() { return PerspectivePosition; }

		inline void SetPerspectiveFront(const glm::vec3 & front) { PerspectiveFront = front; }
		inline glm::vec3& GetPerspectiveFront() { return PerspectiveFront; }

		inline void SetPerspectiveRight(const glm::vec3 & right) { PerspectiveRight = right; }
		inline glm::vec3& GetPerspectiveRight() { return PerspectiveRight; }

		inline void SetPerspectiveUp(const glm::vec3 & up) { PerspectiveUp = up; }
		inline glm::vec3& GetPerspectiveUp() { return PerspectiveUp; }

		inline void SetZoomLevel(float zoom) { m_ZoomLevel = zoom; }
		inline float GetZoomLevel() { return m_ZoomLevel; }

		inline void SetAspectRatio(float aspect) { m_AspectRatio = aspect; }
		inline float GetAspectRatio() { return m_AspectRatio; }

		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		void UpDateViewProjectionMatrix();
		void UpDateViewMatrix(glm::vec3 WorldUp);
		void UpDateProjectionMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		//计算相机坐标的正交基
		glm::vec3 PerspectivePosition;
		glm::vec3 PerspectiveFront;
		glm::vec3 PerspectiveUp;
		glm::vec3 PerspectiveRight;

		float m_ZoomLevel;
		float m_AspectRatio;
	};
}



