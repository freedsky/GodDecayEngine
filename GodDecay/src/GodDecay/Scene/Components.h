#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			//Ӧ�������ǵľ�����ݲ�ͬ�����Դ���ת�Ŷ�
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
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
