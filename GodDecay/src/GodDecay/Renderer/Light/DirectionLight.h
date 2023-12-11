#pragma once
#include "Light.h"

/// <summary>
/// 定向光源
/// 该类型光源并没有光照衰减
/// 
/// 该模式下能显示：灯的颜色/定向光的环境光、漫反射颜色、高光颜色、高光强度、它的位置和旋转都有transform组件，这里也不重复定义
/// 
/// Direction光的lightColor和LightAmbient是同源的，如果要做特殊处理可以单独返回一个颜色以增强艺术性
/// </summary>

namespace GodDecay 
{
	class DirectionLight : public Light
	{
	public:
		DirectionLight();
		~DirectionLight() override;

		void SetLightColor(const glm::vec4& color) override;
		glm::vec4& GetLightColor() override;
	private:
	};
}

