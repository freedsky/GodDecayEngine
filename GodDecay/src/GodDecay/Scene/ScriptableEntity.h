#pragma once

#include "Entity.h"

/// <summary>
/// C++内部脚本类
/// </summary>
namespace GodDecay
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		//脚本中定义的一些接口调用
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(float delteTime) {}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}