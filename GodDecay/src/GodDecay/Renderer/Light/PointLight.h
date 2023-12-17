#pragma once
#include "Light.h"

/// <summary>
/// ���Դ
/// 
/// ��ӵ��Ĺ���˥������[��Щ˥���������ڲ�����ģ��޷��Զ������]
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

