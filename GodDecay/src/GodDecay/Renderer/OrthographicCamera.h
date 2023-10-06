#pragma once

#include <glm/glm.hpp>

namespace GodDecay 
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float n, float f);
		~OrthographicCamera() = default;

		void SetOrthographicProjection(float left, float right, float bottom, float top, float n, float f);

		inline void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
		inline float GeRotation() const { return m_Rotation; }

		inline void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		inline const glm::vec3& GetPosition() const { return m_Position; }

		inline void SetNearPlan(float n) { NearPlan = n; }
		inline float GetNearPlan() { return NearPlan; }

		inline void SetFarPlan(float f) { FarPlan = f; }
		inline float GetFarPlan() { return FarPlan; }

		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		//更新位置或者旋转
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f,0.0f,0.0f };
		float m_Rotation = 0.0f;
		float NearPlan, FarPlan;
	};
}



