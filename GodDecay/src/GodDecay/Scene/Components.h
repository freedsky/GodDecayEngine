#pragma once
#include <string>
#include <glm/glm.hpp>

#include "SceneCamera.h"
#include "ScriptableEntity.h"

/// <summary>
/// 组件Components
/// </summary>

namespace GodDecay
{
	//tag组件
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	//transform组件position rotate scale
	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform) : Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	//Render渲染组件[3D的话估计要加上Mesh组件]
	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f,1.0f,1.0f,1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
	};

	//Camera相机组件
	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;//此变量决定如果场景中有多个相机，我们该启用那些相机
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	//Script组件(内部实现)
	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		//定义脚本对象的引用，会在创建脚本时创建出该泛型对应的对象
		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		//绑定回调
		template<typename T>
		void Bind()
		{
			InstantiateScript = []() {return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}