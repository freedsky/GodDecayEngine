#pragma once

#include "GodDecay/Core/Base.h"

#include "GodDecay/Scene/Scene.h"

/// <summary>
/// ʹ��yaml-cpp���Scene�����е�ʵ��������л����ݲ���
/// ͬʱҲ�ɶ�yaml�ļ��е����ݽ��з����л�����
/// </summary>
namespace GodDecay 
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		//���л�
		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);
		//�����л�
		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		Ref<Scene> m_Scene;
	};
}



