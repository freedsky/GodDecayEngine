#include "ContentBrowserPanel.h"
#include "GodDecay/Core/GodDecayMouseButtonCodes.h"

#include <imgui.h>

namespace GodDecay
{
	//���屻����ļ��е�����
	static const std::filesystem::path s_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		:m_CurrentDirectory(s_AssetPath)
	{
		//��������ͼ��
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		//������һ��
		if (m_CurrentDirectory != std::filesystem::path(s_AssetPath))
		{
			if (ImGui::Button("Renture"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}
		//����ͼ��Ĵ�С
		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		//����ͼ�����Ŵ�С
		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		//��������
		ImGui::Columns(columnCount, 0, false);

		//�������ļ����µ������ļ�
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			//�õ��ļ����µ��ļ����Ʋ�չʾ
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_AssetPath);
			std::string filenameString = relativePath.filename().string();

			//�����ļ������ļ���������ͼ����������
			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			//�������ڵ�ǰ�����У����ҵ���������㴥��
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(GODDECAY_MOUSE_BUTTON_LEFT))
			{
				//if (directoryEntry.is_directory())
				//{
				//	//����ļ������滹���ļ��У���ô����ȥ����������ļ�
				//	if (ImGui::Button(filenameString.c_str()))
				//	{
				//		m_CurrentDirectory /= path.filename();
				//	}
				//}
				//else
				//{
				//	//������ʼ۾�չʾ��������Ҳ���Խ��д򿪵Ȳ�������������ʱ����
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