#include "gdpch.h"
#include "SpotLight.h"

namespace GodDecay 
{
	SpotLight::SpotLight()
	{
		//还是初始化光源的各属性值
		SetLightPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		SetLightRotatetion(glm::vec3(0.0f, 0.0f, 0.0f));

		//环境光应该是定向光源提供的，所有其他类型的光源是不提供amnient的
		SetLightColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		//漫反射和反射光部分可以自定义
		SetLightDiffuse(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		SetLightSpecular(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		SetLightShininess(32.0f);
		SetLightType(LightType::Spot);
		//初始化光照衰减参数
		LightConstant = 1.0f;
		LightLinear = 0.09f;
		LightQuadratic = 0.032f;
		//内外圈的初始化
		LightCutOff = 12.5f;
		LightOuterCutOff = 17.5f;
	}

	SpotLight::~SpotLight()
	{

	}
}
