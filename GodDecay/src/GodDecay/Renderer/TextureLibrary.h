#pragma once
#include "Texture.h"

#include <string>
#include <unordered_map>

/// <summary>
/// textures加载库
/// 每当有Texture加入到内存时，存储在库中
/// 
/// 考虑到不同类型的Textrue怎么办？
/// 
/// 增加立方体贴图
/// </summary>
namespace GodDecay 
{
	class TextureLibrary
	{
	public:
		enum class TextureType 
		{
			T2D,
			TCube
		};

	public:
		//加载2D纹理
		void AddTexture2D(const std::string& name, const Ref<Texture2D>& texture);
		void AddTexture2D(const Ref<Texture2D>& texture);

		Ref<Texture2D> LoadTexture2D(const std::string& name, const std::string& path);
		Ref<Texture2D> LoadTexture2D(const std::string& name, uint32_t width, uint32_t height);

		//加载Cube纹理
		void AddTextureCube(const std::string& name, const Ref<TextureCube>& texture);
		void AddTextureCube(const Ref<TextureCube>& texture);

		Ref<TextureCube> LoadTextureCube(const std::string& name, std::vector<std::string> paths);
		Ref<TextureCube> LoadTextureCube(const std::string& name, uint32_t width, uint32_t height);

		//根据key修改key对应的value
		void UpdataTexture2D(const std::string& T_name, Ref<Texture2D> texture);
		void UpdataTextureCube(const std::string& T_name, Ref<TextureCube> texture);

		Ref<Texture2D> GetTexture2D(const std::string& name);
		Ref<TextureCube> GetTextureCube(const std::string& name);
	
		bool Exists(const std::string& name, TextureType type) const;

		inline std::unordered_map<std::string, Ref<Texture2D>>& GetTexture2DLibraries() { return m_Texture2DLibrary; }
		inline std::unordered_map<std::string, Ref<TextureCube>>& GetTextureCubeLibraries() { return m_TextureCubeLibrary; }
	private:
		//2D纹理库
		std::unordered_map<std::string, Ref<Texture2D>> m_Texture2DLibrary;
		std::unordered_map<std::string, Ref<TextureCube>> m_TextureCubeLibrary;
	};
}



