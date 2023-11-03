#pragma once

#include <string>

/// <summary>
/// 应用于不同平台的文件打开和保存
/// </summary>
namespace GodDecay
{
	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}