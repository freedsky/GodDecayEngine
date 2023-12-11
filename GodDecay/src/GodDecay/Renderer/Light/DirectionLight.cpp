#include "gdpch.h"
#include "DirectionLight.h"

#include "GodDecay/Renderer/SceneLightController.h"

namespace GodDecay 
{
	DirectionLight::DirectionLight()
	{
		//初始化定向光源的各参数
		SetLightPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		SetLightRotatetion(glm::vec3(0.0f, 0.0f, 0.0f));

		//定向光的颜色和环境光同源[这里的ambient做了单独返回的处理提高可定制性]
		SetLightColor(SceneLightController::GetEnvironmentLight());
		SetLightAmbient(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));//如何计算颜色强度
		//漫反射和反射光部分可以自定义
		SetLightDiffuse(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		SetLightSpecular(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		SetLightShininess(32.0f);
		SetLightType(LightType::Direction);
	}

	DirectionLight::~DirectionLight()
	{

	}

	//========================================================

	void DirectionLight::SetLightColor(const glm::vec4& color)
	{
		SceneLightController::SetEnvironmentLight(color);
	}

	glm::vec4& DirectionLight::GetLightColor()
	{
		return SceneLightController::GetEnvironmentLight();
	}
}
