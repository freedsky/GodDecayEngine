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
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	//Render��Ⱦ���[3D�Ļ�����Ҫ����Mesh���]//���������֧��
	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f,1.0f,1.0f,1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

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

	//Mesh����������ݵļ��غͳ�ʼ��
	struct MeshComponent 
	{
		Mesh m_Mesh;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(const Mesh& mesh) : m_Mesh(mesh) {}
	};

	//MeshRender����������mesh����е�������֯Ϊ��������Ҫ�Ķ����������ݣ������л���
	struct MeshRenderComponent 
	{
		//�����mesh�ݲ����ǡ�
		MeshRenderer m_Mesh;

		MeshRenderComponent() = default;
		MeshRenderComponent(const MeshRenderComponent&) = default;
		MeshRenderComponent(const MeshRenderer& mesh) : m_Mesh(mesh) {}
	};
	
	struct LightComponent 
	{
		//��̬��̬�����ƹ������[Ĭ��Ϊ���Դ����ֻ�����˶��򣬺�����޸ĵ�]
		Ref<Light> light = CreateRef<DirectionLight>();

		LightComponent() = default;
		LightComponent(const LightComponent&) = default;
		LightComponent(const Ref<Light>& light) : light(light) {}
	};
}