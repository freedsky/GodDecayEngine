#pragma once

#include "Entity.h"

/// <summary>
/// C++�ڲ��ű���
/// </summary>
namespace GodDecay
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		//�ű��ж����һЩ�ӿڵ���
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