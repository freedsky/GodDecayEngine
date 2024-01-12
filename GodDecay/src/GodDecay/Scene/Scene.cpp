#include "gdpch.h"
#include "Scene.h"

#include <glm/glm.hpp>

#include "Entity.h"
#include "Components.h"
#include "GodDecay/Renderer/Renderer2D.h"
#include "GodDecay/Renderer/Renderer3D.h"
#include "GodDecay/Renderer/SceneLightController.h"
#include "GodDecay//Renderer/SkyBox.h"

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
		//����Ӧ���ǰ���ص����������ͷŵ��ģ���һЩ�Լ������ȫ�ֱ��������Լ�����
		if (entity.HasComponent<LightComponent>()) 
		{
			auto light = entity.GetComponent<LightComponent>();
			if (light.light->GetLightType() == LightType::Direction)
				SceneLightController::DirectionNum--;
			else if (light.light->GetLightType() == LightType::Point)
				SceneLightController::PointNum--;
			else if (light.light->GetLightType() == LightType::Spot)
				SceneLightController::SpotNum--;
		}

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

			SkyBox::GetInstance()->Update(*mainCamera, cameraTransform);

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

		//ÿ�α������ռ��ϣ�ͨ�������light�����ҵ������ж��ڵ�light����Ȼ����и��¡��ڱ�������ʱ��Ϊ������û����ӳ���������������ˣ������ڱ���ʱ�������������ϵ������
		auto groupLight = m_Registry.view<LightComponent>();
		for (auto ob : groupLight) 
		{
			auto [light, transfrom] = m_Registry.get<LightComponent, TransformComponent>(ob);
			auto lightObject = SceneLightController::FindLightFormSceneLights(light.light);
			lightObject->SetLightPosition(transfrom.Translation);
			lightObject->SetLightRotatetion(transfrom.Rotation);
			//GD_ENGINE_INFO("POSITION = {0} , ROTATION = {1}", lightObject->GetLightPosition().x, lightObject->GetLightRotatetion().x);
		}
		//----------------------------------------------------------------

		for (auto entity : group3D) 
		{
			auto [transfrom, mesh, meshrenderer] = m_Registry.get<TransformComponent, MeshComponent, MeshRenderComponent>(entity);
			
			//Renderer3D::DrawCubeMeshRenderer(transfrom.GetTransform(), mesh, (int)entity);
			meshrenderer.m_Mesh.BeginDrawMesh(camera, mesh.m_Mesh.GetMeshType(), mesh.m_Mesh.GetModelPath());

			meshrenderer.m_Mesh.DrawMesh(transfrom.GetTransform());

			meshrenderer.m_Mesh.EndDrawMesh();
		}
		//GD_ENGINE_INFO(SceneLightController::GetSceneLights().size());

		//���ǵ���ȶ�д���⣬��������պ�
			/*
			* Bug����Ϊ��farmebuffer������ָ������岢��ȡ���е������������պ�ҲҪ���ID�Թ���ȡ��ֵ
			* ��Ϊ��ȡ��ʵ��ID������պв�����ʵ��[ʵ��ID��0��ʼ����]�������պе�ID�������Ϊ-1��
			*/
		SkyBox::GetInstance()->Update(camera);
	}

	void Scene::OnUpdateDepth(const Ref<Shader>& shadowShader)
	{
		//����Ⱦʵ�����Ӱ==============================================================================================
		auto group3D = m_Registry.view<TransformComponent, MeshRenderComponent>();

		for (auto entity : group3D)
		{
			auto [transfrom, mesh, meshrenderer] = m_Registry.get<TransformComponent, MeshComponent, MeshRenderComponent>(entity);

			meshrenderer.m_Mesh.DrawShadow(shadowShader, transfrom.GetTransform());
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

	template<typename T>
	void Scene::OnComponentRemove(Entity entity, T& component)
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

	template<>
	void Scene::OnComponentAdded<LightComponent>(Entity entity, LightComponent& component)
	{
		/*
		* Ҫ���������
		* һ���������ʽ��ӵ�
		* �����Ըı�״̬��ʽ
		* ��ΪĬ���ǵ��Դ���Խ��ڵ�һ����ȥ�ж�
		*/

		//�鷳һ�㣬�������������⣬������������ƾ�����ɾ����
		if (SceneLightController::LightCount >= 4) 
		{
			entity.RemoveComponent<LightComponent>();
			GD_ENGINE_WARN("LightNumber Out Index Now Already Remove");
		}
		//�ڴ����ƹ����ʱ�������뵽ȫ�ֵĵƹ⼯����
		SceneLightController::AddLightToSceneLights(component.light);
	}

	//ɾ�����ʱ==========================================================================================
	template<>
	void Scene::OnComponentRemove<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}
	template<>
	void Scene::OnComponentRemove<CameraComponent>(Entity entity, CameraComponent& component)
	{
	}

	template<>
	void Scene::OnComponentRemove<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentRemove<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentRemove<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentRemove<MeshComponent>(Entity entity, MeshComponent& component)
	{
	}

	template<>
	void Scene::OnComponentRemove<MeshRenderComponent>(Entity entity, MeshRenderComponent& component)
	{
	}

	template<>
	void Scene::OnComponentRemove<LightComponent>(Entity entity, LightComponent& component)
	{
		SceneLightController::RemoveLightFormSceneLights(component.light);
	}
}
