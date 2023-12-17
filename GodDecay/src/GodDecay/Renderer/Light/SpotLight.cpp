#include "gdpch.h"
#include "SpotLight.h"

namespace GodDecay 
{
	SpotLight::SpotLight()
	{
		//���ǳ�ʼ����Դ�ĸ�����ֵ
		SetLightPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		SetLightRotatetion(glm::vec3(0.0f, 0.0f, 0.0f));

		//������Ӧ���Ƕ����Դ�ṩ�ģ������������͵Ĺ�Դ�ǲ��ṩamnient��
		SetLightColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		//������ͷ���ⲿ�ֿ����Զ���
		SetLightDiffuse(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		SetLightSpecular(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		SetLightShininess(32.0f);
		SetLightType(LightType::Spot);
		//��ʼ������˥������
		LightConstant = 1.0f;
		LightLinear = 0.09f;
		LightQuadratic = 0.032f;
		//����Ȧ�ĳ�ʼ��
		LightCutOff = 12.5f;
		LightOuterCutOff = 17.5f;
	}

	SpotLight::~SpotLight()
	{

	}
}
