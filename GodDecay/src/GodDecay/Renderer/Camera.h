#pragma once

/// <summary>
/// 抽象相机类[可能会有不同种类的渲染相机]
/// </summary>

#include <glm/glm.hpp>

namespace GodDecay
{
	class Camera 
	{
	public:
		Camera() = default;
		Camera(const glm::mat4 & projection) :m_Projection(projection)
		{}

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};
}