#include "gdpch.h"
#include "Scene.h"

#include <glm/glm.hpp>

#include "Entity.h"
#include "Components.h"
#include "GodDecay/Renderer/Renderer2D.h"
#include "GodDecay/Renderer/Renderer3D.h"

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

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	//entity��Ⱦ
	void Scene::OnUpdateRuntime(float deltaTime)
	{
		//���½ű�
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();

					nsc.Instance->m_Entity = Entity{ entity,this };

					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(deltaTime);
			});
		}

		//�ȴ���һ��Ĭ�����,������Ϊ�����
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;

		//�Ⱦ�����Ⱦ���
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		//��ȥ���ݸ����ȥ��Ⱦ����
		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}

			Renderer2D::EndScene();

			auto group3D = m_Registry.view<TransformComponent, MeshRenderComponent>();
	
			for (auto entity : group3D)
			{
				auto [transfrom, mesh, meshrenderer] = m_Registry.get<TransformComponent, MeshComponent, MeshRenderComponent>(entity);
				
				meshrenderer.m_Mesh.BeginDrawMesh(mainCamera->GetProjection(), cameraTransform, mesh.m_Mesh.GetMeshType());

				meshrenderer.m_Mesh.DrawMesh(transfrom.GetTransform());

				meshrenderer.m_Mesh.EndDrawMesh();
			}

		}
	}

	void Scene::UpdateEditor(float deltaTime, EditorCamera& camera)
	{
		//�ó������������
		//2D��Ⱦ
		Renderer2D::BeginScene(camera);

		auto group2D = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group2D)
		{
			auto [transform, sprite] = group2D.get<TransformComponent, SpriteRendererComponent>(entity);
			
			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}

		Renderer2D::EndScene();

		auto group3D = m_Registry.view<TransformComponent,MeshRenderComponent>();

		for (auto entity : group3D) 
		{
			auto [transfrom, mesh, meshrenderer] = m_Registry.get<TransformComponent, MeshComponent, MeshRenderComponent>(entity);
			
			//Renderer3D::DrawCubeMeshRenderer(transfrom.GetTransform(), mesh, (int)entity);
			meshrenderer.m_Mesh.BeginDrawMesh(camera, mesh.m_Mesh.GetMeshType(), mesh.m_Mesh.GetModelPath());

			meshrenderer.m_Mesh.DrawMesh(transfrom.GetTransform());

			meshrenderer.m_Mesh.EndDrawMesh();
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

	//====================================================

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}
	//�ڴ������ʱ��������ǰ�����������С��������Ϊ���ǻ��������Viewport�������ӿڵı仯��
	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component)
	{
		//���������ʱ���Ƚ���һ�γ�ʼ��
		component.m_Mesh.Init(BaseMeshType::CUBE);
	}

	template<>
	void Scene::OnComponentAdded<MeshRenderComponent>(Entity entity, MeshRenderComponent& component)
	{
		//�������ݵĴ���
		auto& mesh = entity.GetComponent<MeshComponent>().m_Mesh;
		component.m_Mesh.LoadMesh(mesh, (int)entity.GetEntity());
	}
}
