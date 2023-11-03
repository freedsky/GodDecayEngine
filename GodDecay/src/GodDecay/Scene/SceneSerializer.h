#pragma once

#include "GodDecay/Core/Base.h"

#include "GodDecay/Scene/Scene.h"

/// <summary>
/// 使用yaml-cpp库对Scene场景中的实体进行序列化数据操作
/// 同时也可对yaml文件中的数据进行反序列化操作
/// </summary>
namespace GodDecay 
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		//序列化
		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);
		//反序列化
		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		Ref<Scene> m_Scene;
	};
}



