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

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual void DeleteTextureMemory() const = 0;

		virtual bool IsLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
	};

	class TextureCube : public Texture 
	{
	public:
		static Ref<TextureCube> Create(std::vector<std::string> faces);
		static Ref<TextureCube> Create(uint32_t width, uint32_t height);
	};
}



