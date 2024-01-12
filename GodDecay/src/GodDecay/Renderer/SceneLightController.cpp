#include "gdpch.h"
#include "SceneLightController.h"

namespace GodDecay 
{
	uint32_t SceneLightController::ChangeFlag = 0;
	uint32_t SceneLightController::LightCount = 0;
	int SceneLightController::DirectionNum = 0;
	int SceneLightController::PointNum = 0;
	int SceneLightController::SpotNum = 0;

	glm::vec4 SceneLightController::m_EnvironmentLight;
	float SceneLightController::m_EnvironmentLightIntensity;
	std::vector<Ref<Light>> SceneLightController::m_SceneLights;

	void SceneLightController::Init()
	{
		m_EnvironmentLight = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_EnvironmentLightIntensity = 0.3f;
	}

	bool SceneLightController::AddLightToSceneLights(Ref<Light> light)
	{
		//Direction����ֻ���ڼ����д���һ��
		if (light->GetLightType() == LightType::Direction) 
		{
			for (auto& l : m_SceneLights) 
			{
				if (l->GetLightType() == LightType::Direction)
					return false;
			}
			m_SceneLights.push_back(light);
			DirectionNum++;
			return true;
		}
		//������������͵Ĺ�Դ���Ͳ���[���������������Դ���������ڴ˴������޸�����]
		//����direction���������͹�Դһ��������4��
		if (light->GetLightType() == LightType::Point || light->GetLightType() == LightType::Spot) 
		{
			if (LightCount < 4) 
			{
				LightCount++;
				m_SceneLights.push_back(light);
				if (light->GetLightType() == LightType::Point)
					PointNum++;
				else if (light->GetLightType() == LightType::Spot)
					SpotNum++;
			}
			else
			{
				GD_ENGINE_WARN("Light Count Out Index ,Not Add Light to SceneLights");
				return false;
			}
		}
		
		return true;
	}

	bool SceneLightController::RemoveLightFormSceneLights(Ref<Light> light)
	{
		if (m_SceneLights.size() == 0)
			return false;
		std::vector<Ref<Light>>::iterator it = std::find(m_SceneLights.begin(), m_SceneLights.end(), light);
		if (it != m_SceneLights.end()) //end����ָ�򼯺������һλԪ�صĻ���һλ
		{
			//�ж�һ�¹�Դ���ͣ������������
			if (light->GetLightType() == LightType::Point || light->GetLightType() == LightType::Spot) 
			{
				m_SceneLights.erase(it);
				LightCount--;
				if (light->GetLightType() == LightType::Point)
					PointNum--;
				else if (light->GetLightType() == LightType::Spot)
					SpotNum--;
			}
			else
			{
				m_SceneLights.erase(it);
				DirectionNum--;
			}
			
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