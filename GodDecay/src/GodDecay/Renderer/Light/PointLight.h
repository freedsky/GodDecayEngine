#pragma once
#include "Light.h"

/// <summary>
/// 点光源
/// 
/// 添加点光的光照衰减属性[这些衰减属性是内部定义的，无法自定义更改]
/// </summary>

namespace GodDecay 
{
	class PointLight : public Light
	{
	public:
		PointLight();
		~PointLight() override;

		inline void SetLightConstant(float constant) { LightConstant = constant; }
		inline float& GetLightConstant() { return LightConstant; }
		inline void SetLightLinear(float linear) { LightLinear = linear; }
		inline float& GetLightLinear() { return LightLinear; }
		inline void SetLightQuadratic(float quadratic) { LightQuadratic = quadratic; }
		inline float& GetLightQuadratic() { return LightQuadratic; }

	private:
		float LightConstant;
		float LightLinear;
		float LightQuadratic;
	};
}

