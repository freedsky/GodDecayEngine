#pragma once
#include <string>
#include <glm/glm.hpp>

#include "SceneCamera.h"
#include "ScriptableEntity.h"

/// <summary>
/// ���Components
/// </summary>

namespace GodDecay
{
	//tag���
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	//transform���position rotate scale
	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform) : Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	//Render��Ⱦ���[3D�Ļ�����Ҫ����Mesh���]
	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f,1.0f,1.0f,1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
	};

	//Camera������
	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;//�˱�����������������ж����������Ǹ�������Щ���
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	//Script���(�ڲ�ʵ��)
	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		//����ű���������ã����ڴ����ű�ʱ�������÷��Ͷ�Ӧ�Ķ���
		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		//�󶨻ص�
		template<typename T>
		void Bind()
		{
			InstantiateScript = []() {return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}