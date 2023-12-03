#include "gdpch.h"
#include "TextureLibrary.h"

namespace GodDecay 
{
	void TextureLibrary::AddTexture2D(const std::string& name, const Ref<Texture2D>& texture)
	{
		GD_ENGINE_ASSERT(!Exists(name), "Texture2D already exists!");
		m_Texture2DLibrary[name] = texture;
	}

	void TextureLibrary::AddTexture2D(const Ref<Texture2D>& texture)
	{
		
		std::string name = texture->GetTextureName();
		GD_ENGINE_ASSERT(!name.compare(""), "This Texture Name Is Empty");
		AddTexture2D(name, texture);
	}

	Ref<Texture2D> TextureLibrary::LoadTexture2D(const std::string& name, const std::string& path)
	{
		Ref<Texture2D> texture = Texture2D::Create(path);
		std::string TexName;
		if (!name.compare("")) 
		{
			TexName = texture->GetTextureName();
		}
		else
		{
			TexName = name;
		}
		
		AddTexture2D(TexName, texture);
		return texture;
	}

	Ref<Texture2D> TextureLibrary::LoadTexture2D(const std::string& name, uint32_t width, uint32_t height)
	{
		Ref<Texture2D> texture = Texture2D::Create(width, height);
		AddTexture2D(name, texture);
		return texture;
	}

	void TextureLibrary::UpdataTexture2D(const std::string& T_name, Ref<Texture2D> texture)
	{
		m_Texture2DLibrary[T_name] = texture;
	}

	Ref<Texture2D> TextureLibrary::Get(const std::string& name)
	{
		GD_ENGINE_ASSERT(Exists(name), "Texture not found!");
		return m_Texture2DLibrary[name];
	}

	bool TextureLibrary::Exists(const std::string& name) const
	{
		return m_Texture2DLibrary.find(name) != m_Texture2DLibrary.end();
	}
}
