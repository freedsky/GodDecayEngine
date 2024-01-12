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
		ImGui::Begin("SkyBox");//第二个参数是显示开启和关闭按钮
		//这里编写和SkyBox有关的逻辑

		//选择天空盒模式
		std::string SkyBoxTypeStrings[] = { "Normal","PureColor","HDR"};
		//设置默认模式
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
					//这里要把模式进行修改
					SkyBox::GetInstance()->SetSkyBoxType((SkyBox::SkyType)i);
					SkyBox::GetInstance()->ChangeSkyBoxType(SkyBox::GetInstance()->GetSkyBoxType());
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		//设置天空盒的颜色
		ImGui::ColorEdit4("Sky Color", glm::value_ptr(SkyBox::GetInstance()->GetSkyBoxColor()));;

		//重新设置天空盒贴图
		/*
		* 要在SkyBox中创建6个用于显示的2dtexture，然后在这里进行循环显示，在换贴图时只替换path集合即可，最后用更新好后的path集合去重新加载cubeTexture缓冲数据
		*/

		std::string name;
		int size = SkyBox::GetInstance()->GetDispalySkyTextures().size();
		for (int i = 0; i < size; ++i) 
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
			ImGui::PushID(i);
			//给每张图起个标题
			
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
				
			//创建一个目标拖拽区域
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
					//创建一个纹理，并把纹理赋值给组件中的Texture值
					//在此之前检测纹理是否被加载成功
					Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
					if (texture->IsLoaded())
					{
						//可以改变原引用指向新的texture对象
						//第一步先把用于显示的纹理进行替换，并对路径集合进行替换
						SkyBox::GetInstance()->GetDispalySkyTextures()[i] = texture;
						SkyBox::GetInstance()->GetPaths()[i] = texturePath.string();
						//然后点击应用按钮
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
		//展示HDR图片
		ImGui::ImageButton((void*)SkyBox::GetInstance()->GetDisplayHDRSkyTexture()->GetRendererID(), ImVec2(300.0f, 300.0f), {0,1}, {1,0});
		//创建一个目标拖拽区域
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
				//创建一个纹理，并把纹理赋值给组件中的Texture值
				//在此之前检测纹理是否被加载成功
				Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
				if (texture->IsLoaded())
				{
					//可以改变原引用指向新的texture对象
					//第一步先把用于显示的纹理进行替换，并对路径集合进行替换
					SkyBox::GetInstance()->GetDisplayHDRSkyTexture() = texture;
					//HDR文件路径替换
					SkyBox::GetInstance()->GetHDRPath() = texturePath.string();
					//然后点击应用按钮
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
		//应用按钮和关闭按钮
		if (ImGui::Button("Apply", ImVec2(100, 50))) 
		{
			/*
			* Bug ： 当图片的像素大小不一致时，会导致立方体纹理加载失败，从而无法显示
			* 解决方法：规定加载纹理像数大小，当加载时与规定不一致，强制将图片缩放到规定大小
			* 或者加载时自己规定自己，加载相同大小的图片
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
