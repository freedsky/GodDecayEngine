#pragma once

#include <string>

#include "GodDecay/Core.h"

/// <summary>
/// 抽象纹理加载
/// 并对纹理的类型进行分类
/// </summary>
namespace GodDecay 
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetChannel() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
	};
}



