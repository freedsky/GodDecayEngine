#pragma once
#include "Light.h"

/// <summary>
/// �����Դ
/// �����͹�Դ��û�й���˥��
/// 
/// ��ģʽ������ʾ���Ƶ���ɫ/�����Ļ����⡢��������ɫ���߹���ɫ���߹�ǿ�ȡ�����λ�ú���ת����transform���������Ҳ���ظ�����
/// 
/// Direction���lightColor��LightAmbient��ͬԴ�ģ����Ҫ�����⴦����Ե�������һ����ɫ����ǿ������
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

