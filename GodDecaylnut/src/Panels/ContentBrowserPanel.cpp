#include "ContentBrowserPanel.h"
#include "GodDecay/Core/GodDecayMouseButtonCodes.h"

#include <imgui.h>

namespace GodDecay
{
	//定义被浏览文件夹的名称
	extern const std::filesystem::path g_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		:m_CurrentDirectory(g_AssetPath)
	{
		//加载纹理图标
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		//返回上一级
		if (m_CurrentDirectory != std::filesystem::path(g_AssetPath))
		{
			if (ImGui::Button("Renture"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}
		//设置图标的大小
		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		//限制图标缩放大小
		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		//设置行列
		ImGui::Columns(columnCount, 0, false);

		//遍历其文件夹下的所有文件
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			//拿到文件夹下的文件名称并展示
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, g_AssetPath);
			std::string filenameString = relativePath.filename().string();

			//根据文件还是文件夹来决定图标纹理类型
			ImGui::PushID(filenameString.c_str());//push一个ID进去，防止拖拽区域的ID重复导致拖拽相同的文件
			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			
			//拖拽区域的源区域
			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}
			ImGui::PopStyleColor();

			//如果鼠标在当前窗口中，并且点击鼠标左键便触发
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(GODDECAY_MOUSE_BUTTON_LEFT))
			{
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();

			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}
		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}
}