#pragma once
#include <filesystem>

#include "GodDecay/Renderer/Texture.h"

/// <summary>
/// չʾ�ļ���������
/// </summary>
namespace GodDecay
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDirectory;
		//�ļ���ͼ����ļ�ͼ��
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};
}

