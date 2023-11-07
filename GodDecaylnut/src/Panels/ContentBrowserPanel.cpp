#include "ContentBrowserPanel.h"
#include "GodDecay/Core/GodDecayMouseButtonCodes.h"

#include <imgui.h>

namespace GodDecay
{
	//定义被浏览文件夹的名称
	static const std::filesystem::path s_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		:m_CurrentDirectory(s_AssetPath)
	{
		//加载纹理图标
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		//返回上一级
		if (m_CurrentDirectory != std::filesystem::path(s_AssetPath))
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
			auto relativePath = std::filesystem::relative(path, s_AssetPath);
			std::string filenameString = relativePath.filename().string();

			//根据文件还是文件夹来决定图标纹理类型
			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			//如果鼠标在当前窗口中，并且点击鼠标左键便触发
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(GODDECAY_MOUSE_BUTTON_LEFT))
			{
				//if (directoryEntry.is_directory())
				//{
				//	//如果文件夹里面还是文件夹，那么继续去遍历里面的文件
				//	if (ImGui::Button(filenameString.c_str()))
				//	{
				//		m_CurrentDirectory /= path.filename();
				//	}
				//}
				//else
				//{
				//	//如果是问价就展示，这里面也可以进行打开等操作，但这里暂时不表
				//	if (ImGui::Button(filenameString.c_str()))
				//	{

				//	}
				//}

				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();

			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
		}
		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}
}