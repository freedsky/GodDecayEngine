#include "ExtensionPanel.h"
#include "GodDecay/Renderer/SkyBox.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <string>
namespace GodDecay 
{
	bool ExtensionPanel::IsOpenSkyBox = false;

	//-------------------------------------
	extern const std::filesystem::path g_AssetPath;

	Ref<ExtensionPanel> ExtensionPanel::m_instance = nullptr;

	Ref<ExtensionPanel> ExtensionPanel::GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = Ref<ExtensionPanel>(new ExtensionPanel);
		}
		return m_instance;
	}

	ExtensionPanel::ExtensionPanel()
	{
	}
	ExtensionPanel::~ExtensionPanel()
	{
	}

	void ExtensionPanel::DisPlayExtensionWindow(ExtensionWidnowType type)
	{
		switch (type)
		{
		case GodDecay::ExtensionPanel::ExtensionWidnowType::None:
			GD_ENGINE_WARN("Not Know This Window , Try to Others");
			break;
		case GodDecay::ExtensionPanel::ExtensionWidnowType::SkyBox:
			if(IsOpenSkyBox)
				SkyBoxSettingWindow();
			break;
		default:
			GD_ENGINE_WARN("Not Know This Extension Window Type");
			break;
		}
	}

	void ExtensionPanel::SkyBoxSettingWindow()
	{
		ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
		ImGui::Begin("SkyBox");//�ڶ�����������ʾ�����͹رհ�ť
		//�����д��SkyBox�йص��߼�

		//ѡ����պ�ģʽ
		std::string SkyBoxTypeStrings[] = { "Normal","PureColor","HDR"};
		//����Ĭ��ģʽ
		std::string currentShaderTypeString = SkyBoxTypeStrings[(int)SkyBox::GetInstance()->GetSkyBoxType()];
		ImGui::Text("Choose SkyBox Type");
		if (ImGui::BeginCombo("SkyBox Type", currentShaderTypeString.c_str()))
		{
			for (int i = 0; i < 3; i++)
			{
				bool isSelected = currentShaderTypeString.compare(SkyBoxTypeStrings[i]);
				if (ImGui::Selectable(SkyBoxTypeStrings[i].c_str(), isSelected))
				{
					currentShaderTypeString = SkyBoxTypeStrings[i];
					//����Ҫ��ģʽ�����޸�
					SkyBox::GetInstance()->SetSkyBoxType((SkyBox::SkyType)i);
					SkyBox::GetInstance()->ChangeSkyBoxType(SkyBox::GetInstance()->GetSkyBoxType());
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		//������պе���ɫ
		ImGui::ColorEdit4("Sky Color", glm::value_ptr(SkyBox::GetInstance()->GetSkyBoxColor()));;

		//����������պ���ͼ
		/*
		* Ҫ��SkyBox�д���6��������ʾ��2dtexture��Ȼ�����������ѭ����ʾ���ڻ���ͼʱֻ�滻path���ϼ��ɣ�����ø��ºú��path����ȥ���¼���cubeTexture��������
		*/

		std::string name;
		int size = SkyBox::GetInstance()->GetDispalySkyTextures().size();
		for (int i = 0; i < size; ++i) 
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
			ImGui::PushID(i);
			//��ÿ��ͼ�������
			
			switch (i) 
			{
			case 0:
				name = "right :";
				break;
			case 1:
				name = "left  :";
				break;
			case 2:
				name = "bottom:";
				break;
			case 3:
				name = "top   :";
				break;
			case 4:
				name = "front :";
				break;
			case 5:
				name = "back  :";
				break;
			}
			ImGui::Text(name.c_str());
			ImGui::SameLine();
			ImGui::ImageButton((void*)SkyBox::GetInstance()->GetDispalySkyTextures()[i]->GetRendererID(), ImVec2(100.0f, 100.0f), { 0,1 }, { 1,0 });
			if (i == 2)
				ImGui::NewLine();
			else
				ImGui::SameLine();
				
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
						//��һ���Ȱ�������ʾ����������滻������·�����Ͻ����滻
						SkyBox::GetInstance()->GetDispalySkyTextures()[i] = texture;
						SkyBox::GetInstance()->GetPaths()[i] = texturePath.string();
						//Ȼ����Ӧ�ð�ť
					}
					else
					{
						GD_ENGINE_WARN("Could not load texture {0}", texturePath.filename().string());
					}
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::PopID();
			ImGui::PopStyleVar();
		}
		ImGui::NewLine();
		//չʾHDRͼƬ
		ImGui::ImageButton((void*)SkyBox::GetInstance()->GetDisplayHDRSkyTexture()->GetRendererID(), ImVec2(300.0f, 300.0f), {0,1}, {1,0});
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
					//��һ���Ȱ�������ʾ����������滻������·�����Ͻ����滻
					SkyBox::GetInstance()->GetDisplayHDRSkyTexture() = texture;
					//HDR�ļ�·���滻
					SkyBox::GetInstance()->GetHDRPath() = texturePath.string();
					//Ȼ����Ӧ�ð�ť
					GD_ENGINE_INFO(texturePath.string());
				}
				else
				{
					GD_ENGINE_WARN("Could not load texture {0}", texturePath.filename().string());
				}
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::NewLine();
		//Ӧ�ð�ť�͹رհ�ť
		if (ImGui::Button("Apply", ImVec2(100, 50))) 
		{
			/*
			* Bug �� ��ͼƬ�����ش�С��һ��ʱ���ᵼ���������������ʧ�ܣ��Ӷ��޷���ʾ
			* ����������涨��������������С��������ʱ��涨��һ�£�ǿ�ƽ�ͼƬ���ŵ��涨��С
			* ���߼���ʱ�Լ��涨�Լ���������ͬ��С��ͼƬ
			*/
			if (SkyBox::GetInstance()->GetCurrentIsHDR()) 
			{
				SkyBox::GetInstance()->Init(SkyBox::GetInstance()->GetHDRPath());
				SkyBox::GetInstance()->ChangeSkyBoxType(SkyBox::SkyType::HDR);
			}
			else
			{
				SkyBox::GetInstance()->Init(SkyBox::GetInstance()->GetPaths());
				SkyBox::GetInstance()->ChangeSkyBoxType(SkyBox::SkyType::Normal);
			}
		}
		ImGui::SameLine(1,400);
		if (ImGui::Button("Close", ImVec2(100, 50))) 
		{
			IsOpenSkyBox = false;
		}
		ImGui::End();
	}
}
