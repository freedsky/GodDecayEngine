#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <string>

#include "GodDecay/Scene/Components.h"
#include "GodDecay/Core/Logger.h"
#include "GodDecay/Renderer/Renderer3D.h"
#include "GodDecay/Renderer/SceneLightController.h"

namespace GodDecay
{
	extern const std::filesystem::path g_AssetPath;
	int ShaderIndex = 0;
	std::vector<std::string> ShaderTypeStrings;

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
	}

	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entityName)
	{
		if (!m_SelectionContext.HasComponent<T>())
		{
			//��entityû�����������ǲ�ȥ���
			if (ImGui::MenuItem(entityName.c_str()))
			{
				//��������meshRenderer֮ǰҪ���mesh���
				if (!m_SelectionContext.HasComponent<MeshComponent>() && !entityName.compare("Mesh Renderer"))
				{
					GD_ENGINE_WARN("this entity not have meshcomponent ,you need add meshcomponent before");
					return;
				}
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		//�õ��ó�����ע�����ѭ���ҵ����е�ʵ��

		m_Context->m_Registry.each([&](auto entityID)
		{
			Entity entity{ entityID,m_Context.get() };
			DrawEntityNode(entity);
		});

		//����������������Ҵ�����ͣ״̬�°�m_SelectionContext[��ǰѡ��entity����]��ʼ��
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		//��������Ҽ�ʱ��ʾ�˵���������˵��е�createȥ����һ��ʵ��
		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_NoOpenOverExistingPopup | ImGuiPopupFlags_MouseButtonRight))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}
		ImGui::End();

		//��ʼ����ʵ����������==================================
		ImGui::Begin("Properties");
		if (m_SelectionContext)
			DrawComponents(m_SelectionContext);
		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionContext = entity;
	}

	//���ģ��[��Float]
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		//����pushID����˼�����Ƕ���ʹ�����ģ�壬����ÿ��ʹ�����Ķ�����һ�������ĸ��壬���ǲ�ϣ���ı�һ����ȫ���ı���
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	//�������ģ��
	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar(
			);
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component")) 
				{
					//���һ��ɾ��Meshǰ�Ƿ����MeshRenderer�����������������������
					
					removeComponent = true;
				}
					
				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent) 
			{
				entity.RemoveComponent<T>();
			}
				
		}
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		//���������ѡ�е�ʵ���Ƿ��ڳ����д��ڣ�������ھͽ���ImGui��ʾλ
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		//���ýڵ�����[��tag]
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			//�ѵ�ǰѡ�е�ʵ������Ϊm_SelectionContext
			m_SelectionContext = entity;
		}

		//��Scene��ѡ���ʵ��ͬ������Ҽ�ȥɾ���ѱ�ʶΪ����Ϊfalse��������Ϊ�˱�֤�������Ⱦ��������
		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			//�൱�ڼ���չʾ�ӽڵ�
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		//������������Ⱦ��ɲ����ȥɾ��ʵ��
		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		//tags�������ʾ
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));//�����ڴ�ռ�
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		//������
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			//����ģ�忪��
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
			DisplayAddComponentEntry<MeshComponent>("Mesh");
			//���MeshRenderer���֮ǰһ��Ҫ�����Mesh���������ᱨ��
			DisplayAddComponentEntry<MeshRenderComponent>("Mesh Renderer");
			DisplayAddComponentEntry<LightComponent>("Light");

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();
		//transform�������ʾ
		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				DrawVec3Control("Translation", component.Translation);
				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.Scale, 1.0f);
			});
		//Camera�������ʾ
		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
			{
				auto& camera = component.Camera;

				ImGui::Checkbox("Primary", &component.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
						camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

					float perspectiveNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectiveNear))
						camera.SetPerspectiveNearClip(perspectiveNear);

					float perspectiveFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectiveFar))
						camera.SetPerspectiveFarClip(perspectiveFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
				}
			});

		//Sprite��UI�������
		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

				//����һ��Ŀ����ק����
				ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
						//����һ��������������ֵ������е�Textureֵ
						//�ڴ�֮ǰ��������Ƿ񱻼��سɹ�
						Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
						if (texture->IsLoaded())
						{
							component.Texture = texture;
						}
						else
						{
							GD_ENGINE_WARN("Could not load texture {0}", texturePath.filename().string());
						}
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
			});

		//Mesh���
		DrawComponent<MeshComponent>("Mesh", entity, [](auto& component)
		{
			//��ȡ���
			auto& mesh = component.m_Mesh;
			const char* MeshTypeStrings[] = { "CUBE", "CIRLE","MODEL" };
			const char* currentMeshTypeString = MeshTypeStrings[(int)mesh.GetMeshType()];
			ImGui::Text("Choose Mesh Type");
			if (ImGui::BeginCombo("MeshType", currentMeshTypeString))
			{
				for (int i = 0; i < 3; i++)
				{
					bool isSelected = currentMeshTypeString == MeshTypeStrings[i];
					if (ImGui::Selectable(MeshTypeStrings[i], isSelected))
					{
						currentMeshTypeString = MeshTypeStrings[i];
						mesh.SetMeshType((BaseMeshType)i);
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			if ((int)mesh.GetMeshType() == 2) 
			{
				ImGui::Button("load model file");
				//�����mesh�����޸ģ�����path��Ա����
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path modelPath = std::filesystem::path(g_AssetPath) / path;
						if (modelPath.extension().string() != ".obj")
						{
							GD_ENGINE_WARN("Could not load {0} - not a obj file", modelPath.filename().string());
							return;
						}
						else
						{
							mesh.SetModelPath(modelPath.string());
						}				
					}
					ImGui::EndDragDropTarget();
				}
				//�Ȱ�mesh���ݽ�������
			}
		});

		DrawComponent<MeshRenderComponent>("Mesh Renderer", entity, [](auto& component) 
		{
			Ref<MeshRenderData> mesh = component.m_Mesh.GetMeshRendererData();
			ImGui::ColorEdit4("Color", glm::value_ptr(mesh->MatrialData.GetMeshColor()));
			
			ImGui::NewLine();

			//ѡ��Shader[���߿��Ա��϶��޸�]
			int S_size = mesh->MatrialData.GetShaderList().GetShaderLibraries().size();
			
			for (auto& key : mesh->MatrialData.GetShaderList().GetShaderLibraries())
			{
				ShaderTypeStrings.push_back(key.first);
			}
			
			std::string currentShaderTypeString = ShaderTypeStrings[ShaderIndex];
			ImGui::Text("Choose Shader");
			if (ImGui::BeginCombo("Shader", currentShaderTypeString.c_str()))
			{
				for (int i = 0; i < S_size; i++)
				{
					bool isSelected = currentShaderTypeString.compare(ShaderTypeStrings[i]);
					if (ImGui::Selectable(ShaderTypeStrings[i].c_str(), isSelected))
					{
						currentShaderTypeString = ShaderTypeStrings[i];
						ShaderIndex = i;
						//����Ӧ�õ���meshRenderer����ı��Shader�ķ���
						component.m_Mesh.ChanageShader(currentShaderTypeString);
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::NewLine();
			//����һ�������б���ʾ����
			int T_size = mesh->MatrialData.GetTextureList(ShaderTypeStrings[ShaderIndex]).GetTexture2DLibraries().size();
			std::vector<std::string> Texname;
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			for (auto& e : mesh->MatrialData.GetTextureList(ShaderTypeStrings[ShaderIndex]).GetTexture2DLibraries())
			{
				Texname.push_back(e.first);
			}
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(-1, -1));
			for (int i = 0; i < T_size; ++i) 
			{
				ImGui::Text(Texname[i].c_str());
				ImGui::ImageButton((void*)mesh->MatrialData.GetTextureList(ShaderTypeStrings[ShaderIndex]).Get(Texname[i])->GetRendererID(), ImVec2(viewportPanelSize.x * 0.6f, 200.0f), { 0,1 }, { 1,0 });
				
				//����һ��Ŀ����ק����
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
						//����һ��������������ֵ������е�Textureֵ
						//�ڴ�֮ǰ��������Ƿ񱻼��سɹ�
						Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
						if (texture->IsLoaded())
						{
							//���Ըı�ԭ����ָ���µ�texture����
							component.m_Mesh.GetMeshRendererData()->MatrialData.GetTextureList(ShaderTypeStrings[ShaderIndex]).UpdataTexture2D(Texname[i], texture);
						}
						else
						{
							GD_ENGINE_WARN("Could not load texture {0}", texturePath.filename().string());
						}
					}
					ImGui::EndDragDropTarget();
				}
			}
			ImGui::PopStyleVar();
		});

		//�ƹ��������ʾ
		DrawComponent<LightComponent>("Light", entity, [](auto& component) 
			{
				//��Դλ�����Եĸ�ֵ
				//��scene����ʱȥ����
				//component

				//�Ƶ���ɫ�ͻ���������ȫ�ֹ�����������ͳһ�����
				ImGui::PushID("1");
				ImGui::Text("Light Color[Global Light]");
				if (ImGui::ColorEdit4("", glm::value_ptr(component.light->GetLightColor()))) 
				{
					SceneLightController::ChangeFlag = 1;
				}
				ImGui::PopID();
				//����ǿ��
				ImGui::Text("EnvironmentIntensity");
				if (ImGui::SliderFloat("Intensity", &SceneLightController::GetEnvironmentLightIntensity(), 0.0f, 1.0f)) 
				{
					SceneLightController::ChangeFlag = 1;
				}
				//ÿ�θı价������ɫ��ǿ��ʱ��ȥ�޸���ʵ�Ļ�����ֵ
				//ImGui::PushID("test");
				//ImGui::Text("Direction Ambient Color");
				//ImGui::ColorEdit4("sdasd", glm::value_ptr(component.light->GetLightAmbient()));
				//ImGui::PopID();
				ImGui::PushStyleColor(1, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
				ImGui::NewLine();
				ImGui::PopStyleColor();
				//������
				ImGui::PushID("2");
				ImGui::Text("Light Diffuse Color");
				if (ImGui::ColorEdit4("", glm::value_ptr(component.light->GetLightDiffuse()))) 
				{
					SceneLightController::ChangeFlag = 1;
				}
				ImGui::PopID();

				//�߹���ɫ
				ImGui::PushID("3");
				ImGui::Text("Light Specular Color");
				if (ImGui::ColorEdit4("", glm::value_ptr(component.light->GetLightSpecular()))) 
				{
					SceneLightController::ChangeFlag = 1;
				}
				ImGui::PopID();

				//�߹�ǿ��
				ImGui::Text("Light Specular Shininess");
				if (ImGui::SliderInt("Shininess", &(int)component.light->GetLightShininess(), 32, 1000)) 
				{
					SceneLightController::ChangeFlag = 1;
				}
			});
	}
}
