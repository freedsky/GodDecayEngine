#pragma once

#include <glm/glm.hpp>

namespace GodDecay::Math 
{
	//转为四元数
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);
}

