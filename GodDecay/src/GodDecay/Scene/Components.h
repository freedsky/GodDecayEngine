#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "SceneCamera.h"
#include "ScriptableEntity.h"
#include "GodDecay/Renderer/Texture.h"
#include "GodDecay/Renderer/Mesh.h"
#include "GodDecay/Renderer/MeshRenderer.h"
#include "GodDecay/Renderer/Light/Light.h"
#include "GodDecay/Renderer/Light/DirectionLight.h"

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
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			//应该是它们的矩阵根据不同轴向以此旋转才对
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	//Render渲染组件[3D的话估计要加上Mesh组件]//增加纹理的支持
	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f,1.0f,1.0f,1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

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

	//Mesh组件负责数据的加载和初始化
	struct MeshComponent 
	{
		Mesh m_Mesh;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(const Mesh& mesh) : m_Mesh(mesh) {}
	};

	//MeshRender组件它负责把mesh组件中的数据组织为绘制所需要的顶点属性数据，并进行绘制
	struct MeshRenderComponent 
	{
		//这里的mesh暂不考虑。
		MeshRenderer m_Mesh;

		MeshRenderComponent() = default;
		MeshRenderComponent(const MeshRenderComponent&) = default;
		MeshRenderComponent(const MeshRenderer& mesh) : m_Mesh(mesh) {}
	};
	
	struct LightComponent 
	{
		//多态动态决定灯光的类型[默认为点光源，但只创建了定向，后面会修改的]
		Ref<Light> light = CreateRef<DirectionLight>();

		LightComponent() = default;
		LightComponent(const LightComponent&) = default;
		LightComponent(const Ref<Light>& light) : light(light) {}
	};
}