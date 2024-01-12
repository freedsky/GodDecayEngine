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

	void Scene::DestroyEntity(Entity entity)
	{
		//里面应该是把相关的组件对象给释放掉的，但一些自己定义的全局变量还得自己操作
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

	//entity渲染
	void Scene::OnUpdateRuntime(float deltaTime)
	{
		//更新脚本
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

		//先创建一个默认相机,并设置为主相机
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;

		//先决定渲染相机
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

		//再去根据该相机去渲染物体
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
		//用场景相机来更新
		//2D渲染
		Renderer2D::BeginScene(camera);

		auto group2D = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group2D)
		{
			auto [transform, sprite] = group2D.get<TransformComponent, SpriteRendererComponent>(entity);
			
			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}

		Renderer2D::EndScene();

		auto group3D = m_Registry.view<TransformComponent,MeshRenderComponent>();

		//每次遍历光照集合，通过组件中light引用找到集合中对于的light对象然后进行更新【在遍历集合时因为集合中没有添加超出而组件又生成了，所以在遍历时会遇到超出集合的情况】
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

		//考虑到深度读写问题，最后绘制天空盒
			/*
			* Bug：因为在farmebuffer中鼠标会指向绘制体并读取其中的像数，因此天空盒也要添加ID以供读取数值
			* 因为读取的实体ID，而天空盒不属于实体[实体ID从0开始递增]，因此天空盒的ID是特殊的为-1；
			*/
		SkyBox::GetInstance()->Update(camera);
	}

	void Scene::OnUpdateDepth(const Ref<Shader>& shadowShader)
	{
		//先渲染实体的阴影==============================================================================================
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
	//在创建相机时，调整当前创建的相机大小，这是因为我们基于相机的Viewport来进行视口的变化的
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
		//在组件加载时，先进行一次初始化
		component.m_Mesh.Init(BaseMeshType::CUBE);
	}

	template<>
	void Scene::OnComponentAdded<MeshRenderComponent>(Entity entity, MeshRenderComponent& component)
	{
		//进行数据的传输
		auto& mesh = entity.GetComponent<MeshComponent>().m_Mesh;
		component.m_Mesh.LoadMesh(mesh, (int)entity.GetEntity());
	}

	template<>
	void Scene::OnComponentAdded<LightComponent>(Entity entity, LightComponent& component)
	{
		/*
		* 要分两种情况
		* 一是以组件形式添加的
		* 二是以改变状态形式
		* 因为默认是点光源所以仅在第一种下去判断
		*/

		//麻烦一点，创建完后立即检测，如果超出了限制就马上删除掉
		if (SceneLightController::LightCount >= 4) 
		{
			entity.RemoveComponent<LightComponent>();
			GD_ENGINE_WARN("LightNumber Out Index Now Already Remove");
		}
		//在创建灯光组件时把它加入到全局的灯光集合中
		SceneLightController::AddLightToSceneLights(component.light);
	}

	//删除组件时==========================================================================================
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
