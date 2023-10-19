#include "gdpch.h"
#include "Scene.h"

#include <glm/glm.hpp>

#include "Entity.h"
#include "Components.h"
#include "GodDecay/Renderer/Renderer2D.h"

namespace GodDecay 
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	/*
	* 一个场景对象包含唯一一个注册表对象m_Registry，然后通过这个注册表去创建Entity对象
	* 通过这个Entity对象可以对实体进行添加，删除，返回它本身的组件引用
	* 也就是以一个场景对象为主，它包含一个entt注册表，这个注册表可创建管理多个实体
	* 而实体是由Entity对象进行管理的，通过这个对象进行对实体的组件进行管理，而一个实体下又有多个组件
	*/
	Entity Scene::CreateEntity(const std::string& name)
	{
		//而这是一个实体必要的组件，所有在这里默认添加了，后续需要其他组件可以通过返回值进行添加
		Entity entity = { m_Registry.create(),this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	//entity渲染
	void Scene::OnUpdata(float deltaTime)
	{
		//先创建一个默认相机,并设置为主相机
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;

		//先决定渲染相机
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		//再去根据该相机去渲染物体
		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform, sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}
}
