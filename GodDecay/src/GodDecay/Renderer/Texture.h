#pragma once

#include <string>

#include "GodDecay/Core/Base.h"

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

		virtual uint32_t GetRendererID() const = 0;

		virtual std::string GetTextureName() const = 0;

		virtual void SetData(void* data, uint32_t size, bool depth = false) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual void DeleteTextureMemory() const = 0;

		virtual bool IsLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path, bool HDR = false);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, bool depth = false);
		//创建一个空纹理，它为外部创建的纹理进行封装[此创建方式仅仅记录在外部创建好的纹理ID以及一些基本纹理属性值的记录]
		static Ref<Texture2D> Create(std::string name, uint32_t textureID, uint32_t width, uint32_t height);
	};

	class TextureCube : public Texture 
	{
	public:
		static Ref<TextureCube> Create(std::vector<std::string> faces);
		static Ref<TextureCube> Create(uint32_t width, uint32_t height, bool HDR = false, bool mipmap = false);
	};
}



