#pragma once
#include "Light.h"

/// <summary>
/// 聚光光源，它同样没有环境光，所有环境光都来之与定向光
/// 
/// </summary>

namespace GodDecay 
{
	class SpotLight : public Light
	{
	public:
		SpotLight();
		~SpotLight() override;

		inline void SetLightConstant(float constant) { LightConstant = constant; }
		inline float& GetLightConstant() { return LightConstant; }
		inline void SetLightLinear(float linear) { LightLinear = linear; }
		inline float& GetLightLinear() { return LightLinear; }
		inline void SetLightQuadratic(float quadratic) { LightQuadratic = quadratic; }
		inline float& GetLightQuadratic() { return LightQuadratic; }

		inline void SetLightCutOff(float cutOff) { LightCutOff = cutOff; }
		inline float& GetLightCutOff() { return LightCutOff; }
		inline void SetLightOuterCutOff(float outCutOff) { LightOuterCutOff = outCutOff; }
		inline float& GetLightOuterCutOff() { return LightOuterCutOff; }

	private:
		//聚光的内外圈限制变量
		float LightCutOff;
		float LightOuterCutOff;
		//光照衰减
		float LightConstant;
		float LightLinear;
		float LightQuadratic;
	};
}



