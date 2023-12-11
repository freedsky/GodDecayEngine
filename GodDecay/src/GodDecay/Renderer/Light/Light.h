#pragma once
#include <glm/glm.hpp>

/// <summary>
/// �����Դ��
/// ��Դ��ɫ��ambient��diffuse��specular��shininess��λ����������Ҫд��������Ȼ������»���鷳
/// </summary>

namespace GodDecay 
{
	enum class LightType 
	{
		Direction,
		Point,
		Spot
	};

	class Light 
	{
	public:
		Light() = default;
		virtual ~Light() = default;

		//�����ĵƵ���ɫ�ͻ������������Ƶ����ò�һ��������Ӧ�ö���Ϊ�鷽���ڶ������ȥ����
		virtual void SetLightColor(const glm::vec4& color) { m_LightColor = color; }
		virtual glm::vec4& GetLightColor() { return m_LightColor; }
		inline void SetLightAmbient(const glm::vec4& ambient) { m_LightAmbient = ambient; }
		inline glm::vec4& GetLightAmbient() { return m_LightAmbient; }

		inline void SetLightPosition(const glm::vec3& position) { m_LightPosition = position; }
		inline glm::vec3& GetLightPosition() { return m_LightPosition; }
		inline void SetLightRotatetion(const glm::vec3& rotatetion) { m_LightRotatetion = rotatetion; }
		inline glm::vec3& GetLightRotatetion() { return m_LightRotatetion; }

		inline void SetLightDiffuse(const glm::vec4& diffuse) { m_LightDiffuse = diffuse; }
		inline glm::vec4& GetLightDiffuse() { return m_LightDiffuse; }
		inline void SetLightSpecular(const glm::vec4& specular) { m_LightSpecular = specular; }
		inline glm::vec4& GetLightSpecular() { return m_LightSpecular; }
		inline void SetLightShininess(uint32_t shininess) { m_LightShininess = shininess; }
		inline uint32_t& GetLightShininess() { return m_LightShininess; }

		//���ڱ���Դ����
		inline void SetLightType(LightType type) { m_LightType = type; }
		inline LightType GetLightType() { return m_LightType; }

		//������ɫ����
		glm::vec4& LightColorLuminance(glm::vec4& lightcolor, float lum)
		{
			GD_ENGINE_INFO(lum);
			glm::vec3 finalColor = glm::vec3(lightcolor.r,lightcolor.g,lightcolor.b) * lum;

			float luminance = 0.2125 * lightcolor.r + 0.7154 * lightcolor.g + 0.0721 * lightcolor.b;
			glm::vec3 luminanceColor = glm::vec3(luminance, luminance, luminance);
			finalColor = luminanceColor * finalColor;

			//�����Աȶ�
			glm::vec3 avgColor = glm::vec3(0.5f, 0.5f, 0.5f);
			finalColor = avgColor * finalColor;
			return glm::vec4(finalColor, lightcolor.a);
		}

	private:
		glm::vec3 m_LightPosition;
		glm::vec3 m_LightRotatetion;

		glm::vec4 m_LightColor;
		glm::vec4 m_LightAmbient;
		glm::vec4 m_LightDiffuse;
		glm::vec4 m_LightSpecular;
		uint32_t m_LightShininess;

		LightType m_LightType;
	};
}
