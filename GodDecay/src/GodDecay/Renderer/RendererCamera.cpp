#include "gdpch.h"
#include "RendererCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GodDecay 
{
	RendererCamera::RendererCamera(float left, float right, float bottom, float top, float n, float f)
		:m_ProjectionMatrix(glm::ortho(left, right, bottom, top, n, f)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	RendererCamera::RendererCamera(float fov, float screenRatio, float n, float f, glm::vec3 position, glm::vec3 target)
		: m_ProjectionMatrix(glm::perspective(fov, screenRatio, n, f)), m_ViewMatrix(glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f)))
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void RendererCamera::RecalculateViewMatrix(bool Is3D)
	{


		if (Is3D)
		{
			//暂时不写，会把相机控制移动到相机控制类中
		}
		else
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_2DPosition) *
				glm::rotate(glm::mat4(1.0f), glm::radians(m_2DRotation), glm::vec3(0, 0, 1));

			m_ViewMatrix = glm::inverse(transform);
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
	}
}
