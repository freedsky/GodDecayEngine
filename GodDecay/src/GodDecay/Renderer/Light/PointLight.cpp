#include "gdpch.h"
#include "PointLight.h"

namespace GodDecay 
{
    PointLight::PointLight()
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
		SetLightType(LightType::Point);
		//初始化光照衰减参数
		LightConstant = 1.0f;
		LightLinear = 0.09f;
		LightQuadratic = 0.032f;
    }

    PointLight::~PointLight()
    {

    }
}
