#pragma once

#include <glm/glm.hpp>

namespace GodDecay 
{
	class RendererCamera
	{
	public:
		RendererCamera(float left, float right, float bottom, float top, float n, float f);
		RendererCamera(float fov, float screenRatio, float n, float f, glm::vec3 position, glm::vec3 target);
		~RendererCamera() = default;

		inline void Set2DRotation(float rotation) { m_2DRotation = rotation; RecalculateViewMatrix(false); }
		inline float Ge2DRotation() const { return m_2DRotation; }

		inline void Set2DPosition(const glm::vec3& position) { m_2DPosition = position; RecalculateViewMatrix(false); }
		inline const glm::vec3& Get2DPosition() const { return m_2DPosition; }

		inline const glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
		inline const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() { return m_ViewProjectionMatrix; }
	private:
		//更新位置或者旋转
		void RecalculateViewMatrix(bool Is3D);
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_2DPosition = { 0.0f,0.0f,0.0f };
		float m_2DRotation = 0.0f;
	};
}



