#include "gdpch.h"
#include "DirectionLight.h"

#include "GodDecay/Renderer/SceneLightController.h"

namespace GodDecay 
{
	DirectionLight::DirectionLight()
	{
		//��ʼ�������Դ�ĸ�����
		SetLightPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		SetLightRotatetion(glm::vec3(0.0f, 0.0f, 0.0f));

		//��������ɫ�ͻ�����ͬԴ[�����ambient���˵������صĴ�����߿ɶ�����]
		SetLightColor(SceneLightController::GetEnvironmentLight());
		SetLightAmbient(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));//��μ�����ɫǿ��
		//������ͷ���ⲿ�ֿ����Զ���
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
