#pragma once
#include "Light.h"

/// <summary>
/// �۹��Դ����ͬ��û�л����⣬���л����ⶼ��֮�붨���
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
		//�۹������Ȧ���Ʊ���
		float LightCutOff;
		float LightOuterCutOff;
		//����˥��
		float LightConstant;
		float LightLinear;
		float LightQuadratic;
	};
}



