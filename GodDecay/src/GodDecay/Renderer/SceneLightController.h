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
		//���ù�Դ������
		static int DirectionNum;
		static int PointNum;
		static int SpotNum;
		//��Դ������
		static uint32_t LightCount;

		//����״̬���Ŀ���
		static uint32_t ChangeFlag;

		static void Init();
		//ÿ���޸�ǿ�ȶ�Ҫ���»�ȡ�����⣬��������Ҫ���Ի�����ǿ�Ȳ��������Ļ�����
		static glm::vec4& GetEnvironmentLight() { return m_EnvironmentLight; }
		static void SetEnvironmentLight(const glm::vec4& light) { m_EnvironmentLight = light; }

		static float& GetEnvironmentLightIntensity() { return m_EnvironmentLightIntensity; }
		static void SetEnvironmentLightIntensity(float intensity) { m_EnvironmentLightIntensity = intensity; }

		static std::vector<Ref<Light>>& GetSceneLights() { return m_SceneLights; }
		//�ӳ����ƹ⼯������ӻ���ɾ����Դ
		static bool AddLightToSceneLights(Ref<Light> light);
		static bool RemoveLightFormSceneLights(Ref<Light> light);
		//���Ҷ���
		static Ref<Light> FindLightFormSceneLights(Ref<Light> light);
	private:
		static glm::vec4 m_EnvironmentLight;
		static float m_EnvironmentLightIntensity;
		//ʵʱ��Դ���ϣ�ÿ������һ����Դʱ�����ᱻ��ӽ��˹�Դ���ϣ�����Թ�Դ������Ⱦ
		static std::vector<Ref<Light>> m_SceneLights;
	};
}

