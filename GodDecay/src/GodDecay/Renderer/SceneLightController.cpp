#include "gdpch.h"
#include "SceneLightController.h"

namespace GodDecay 
{
	uint32_t SceneLightController::ChangeFlag = 0;

	glm::vec4 SceneLightController::m_EnvironmentLight;
	float SceneLightController::m_EnvironmentLightIntensity;
	std::vector<Ref<Light>> SceneLightController::m_SceneLights;

	void SceneLightController::Init()
	{
		m_EnvironmentLight = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_EnvironmentLightIntensity = 1.0f;
	}

	bool SceneLightController::AddLightToSceneLights(Ref<Light> light)
	{
		//Direction类型只能在集合中存在一个
		if (light->GetLightType() == LightType::Direction && m_SceneLights.size() != 0) 
		{
			for (auto& l : m_SceneLights) 
			{
				if (l->GetLightType() == LightType::Direction)
					return false;
			}
			m_SceneLights.push_back(light);
			return true;
		}
		//如果是其他类型的光源，就不管[如果后期来调整光源数量可以在此处进行修改限制]
		m_SceneLights.push_back(light);
		return true;
	}

	bool SceneLightController::RemoveLightFormSceneLights(Ref<Light> light)
	{
		if (m_SceneLights.size() == 0)
			return false;
		std::vector<Ref<Light>>::iterator it = std::find(m_SceneLights.begin(), m_SceneLights.end(), light);
		if (it != m_SceneLights.end()) //end函数指向集合中最后一位元素的还后一位
		{
			m_SceneLights.erase(it);
			return true;
		}
		return false;
	}

	Ref<Light> SceneLightController::FindLightFormSceneLights(Ref<Light> light) 
	{
		if (m_SceneLights.size() == 0)
			return nullptr;
		std::vector<Ref<Light>>::iterator it = std::find(m_SceneLights.begin(), m_SceneLights.end(), light);
		if (it == m_SceneLights.end())
			return nullptr;
		return *it;
	}

}