#pragma once

#include <string>

/// <summary>
/// Ӧ���ڲ�ͬƽ̨���ļ��򿪺ͱ���
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