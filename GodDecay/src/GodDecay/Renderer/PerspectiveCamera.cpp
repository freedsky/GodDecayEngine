#include "gdpch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GodDecay 
{
	PerspectiveCamera::PerspectiveCamera(float zoom, float aspect, float n, float f)
		:PerspectivePosition(0.0f, 0.0f, 2.0f), PerspectiveFront(0.0f, 0.0f, -1.0f), PerspectiveUp(0.0f, 1.0f, 0.0f), PerspectiveRight(0.0f)
	{
		m_ZoomLevel = zoom;
		m_AspectRatio = aspect;

		m_ProjectionMatrix = glm::perspective(glm::radians(m_ZoomLevel), m_AspectRatio, n, f);
		m_ViewMatrix = glm::lookAt(PerspectivePosition, PerspectivePosition + PerspectiveFront, PerspectiveUp);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void PerspectiveCamera::UpDateViewProjectionMatrix()
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::UpDateViewMatrix(glm::vec3 WorldUp)
	{
		PerspectiveRight = glm::normalize(glm::cross(PerspectiveFront, WorldUp));
		PerspectiveUp = glm::cross(PerspectiveRight, PerspectiveFront);

		m_ViewMatrix = glm::lookAt(PerspectivePosition, PerspectivePosition + PerspectiveFront, PerspectiveUp);
	}

	void PerspectiveCamera::UpDateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_ZoomLevel), m_AspectRatio, 0.01f, 100.0f);
	}
}
