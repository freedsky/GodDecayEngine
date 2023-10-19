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
	* һ�������������Ψһһ��ע������m_Registry��Ȼ��ͨ�����ע���ȥ����Entity����
	* ͨ�����Entity������Զ�ʵ�������ӣ�ɾ����������������������
	* Ҳ������һ����������Ϊ����������һ��enttע������ע���ɴ���������ʵ��
	* ��ʵ������Entity������й���ģ�ͨ�����������ж�ʵ���������й�����һ��ʵ�������ж�����
	*/
	Entity Scene::CreateEntity(const std::string& name)
	{
		//������һ��ʵ���Ҫ�����������������Ĭ������ˣ�������Ҫ�����������ͨ������ֵ�������
		Entity entity = { m_Registry.create(),this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	//entity��Ⱦ
	void Scene::OnUpdata(float deltaTime)
	{
		//�ȴ���һ��Ĭ�����,������Ϊ�����
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;

		//�Ⱦ�����Ⱦ���
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

		//��ȥ���ݸ����ȥ��Ⱦ����
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
