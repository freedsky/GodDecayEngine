#include "gdpch.h"
#include "PointLight.h"

namespace GodDecay 
{
    PointLight::PointLight()
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
		SetLightType(LightType::Point);
		//��ʼ������˥������
		LightConstant = 1.0f;
		LightLinear = 0.09f;
		LightQuadratic = 0.032f;
    }

    PointLight::~PointLight()
    {

    }
}
