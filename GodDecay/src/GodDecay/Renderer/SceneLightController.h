#pragma once
#include "GodDecay/Core/Base.h"
#include "GodDecay/Renderer/Light/Light.h"

#include <glm/glm.hpp>

#include <vector>

namespace GodDecay 
{
	class SceneLightController
	{
	public:
		//设置光源计数器
		static int DirectionNum;
		static int PointNum;
		static int SpotNum;
		//光源计数器
		static uint32_t LightCount;

		//设置状态更改开关
		static uint32_t ChangeFlag;

		static void Init();
		//每次修改强度都要重新获取环境光，而环境光要乘以环境光强度才是真正的环境光
		static glm::vec4& GetEnvironmentLight() { return m_EnvironmentLight; }
		static void SetEnvironmentLight(const glm::vec4& light) { m_EnvironmentLight = light; }

		static float& GetEnvironmentLightIntensity() { return m_EnvironmentLightIntensity; }
		static void SetEnvironmentLightIntensity(float intensity) { m_EnvironmentLightIntensity = intensity; }

		static std::vector<Ref<Light>>& GetSceneLights() { return m_SceneLights; }
		//从场景灯光集合中添加或者删除光源
		static bool AddLightToSceneLights(Ref<Light> light);
		static bool RemoveLightFormSceneLights(Ref<Light> light);
		//查找对象
		static Ref<Light> FindLightFormSceneLights(Ref<Light> light);
	private:
		static glm::vec4 m_EnvironmentLight;
		static float m_EnvironmentLightIntensity;
		//实时光源集合，每当创建一个光源时，都会被添加进此光源集合，方便对光源进行渲染
		static std::vector<Ref<Light>> m_SceneLights;
	};
}

