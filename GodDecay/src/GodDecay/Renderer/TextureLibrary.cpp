#include "gdpch.h"
#include "TextureLibrary.h"

namespace GodDecay 
{
	void TextureLibrary::AddTexture2D(const std::string& name, const Ref<Texture2D>& texture)
	{
		GD_ENGINE_ASSERT(!Exists(name,TextureType::T2D), "Texture2D already exists!");
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

	//======================================================================================

	void TextureLibrary::AddTextureCube(const std::string& name, const Ref<TextureCube>& texture)
	{
		GD_ENGINE_ASSERT(!Exists(name, TextureType::TCube), "TextureCube already exists!");
		m_TextureCubeLibrary[name] = texture;
	}

	void TextureLibrary::AddTextureCube(const Ref<TextureCube>& texture)
	{
		std::string name = texture->GetTextureName();
		GD_ENGINE_ASSERT(!name.compare(""), "This Texture Name Is Empty");
		AddTextureCube(name, texture);
	}

	Ref<TextureCube> TextureLibrary::LoadTextureCube(const std::string& name, std::vector<std::string> paths)
	{
		Ref<TextureCube> texture = TextureCube::Create(paths);
		std::string TexName;
		if (!name.compare(""))
		{
			TexName = texture->GetTextureName();
		}
		else
		{
			TexName = name;
		}

		AddTextureCube(TexName, texture);
		return texture;
	}

	Ref<TextureCube> TextureLibrary::LoadTextureCube(const std::string& name, uint32_t width, uint32_t height)
	{
		Ref<TextureCube> texture = TextureCube::Create(width, height);
		AddTextureCube(name, texture);
		return texture;
	}

	void TextureLibrary::UpdataTexture2D(const std::string& T_name, Ref<Texture2D> texture)
	{
		m_Texture2DLibrary[T_name] = texture;
	}

	void TextureLibrary::UpdataTextureCube(const std::string& T_name, Ref<TextureCube> texture)
	{
		m_TextureCubeLibrary[T_name] = texture;
	}

	Ref<Texture2D> TextureLibrary::GetTexture2D(const std::string& name)
	{
		GD_ENGINE_ASSERT(Exists(name, TextureType::T2D), "Texture not found!");
		return m_Texture2DLibrary[name];
	}

	Ref<TextureCube> TextureLibrary::GetTextureCube(const std::string& name)
	{
		GD_ENGINE_ASSERT(Exists(name, TextureType::TCube), "Texture not found!");
		return m_TextureCubeLibrary[name];
	}

	bool TextureLibrary::Exists(const std::string& name, TextureType type) const
	{
		bool e = false;
		if (type == TextureType::T2D)
			e = m_Texture2DLibrary.find(name) != m_Texture2DLibrary.end();
		else if(type == TextureType::TCube)
			e = m_TextureCubeLibrary.find(name) != m_TextureCubeLibrary.end();
		return e;
	}
}
