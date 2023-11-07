#pragma once
#include <filesystem>

#include "GodDecay/Renderer/Texture.h"

/// <summary>
/// 展示文件浏览的面板
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
		//文件夹图标和文件图标
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};
}

