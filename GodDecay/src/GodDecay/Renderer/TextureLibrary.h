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
/// </summary>
namespace GodDecay 
{
	class TextureLibrary
	{
	public:
		//加载2D纹理
		void AddTexture2D(const std::string& name, const Ref<Texture2D>& texture);
		void AddTexture2D(const Ref<Texture2D>& texture);

		Ref<Texture2D> LoadTexture2D(const std::string& name, const std::string& path);
		Ref<Texture2D> LoadTexture2D(const std::string& name, uint32_t width, uint32_t height);

		//根据key修改key对应的value
		void UpdataTexture2D(const std::string& T_name, Ref<Texture2D> texture);

		Ref<Texture2D> Get(const std::string& name);
	
		bool Exists(const std::string& name) const;

		inline std::unordered_map<std::string, Ref<Texture2D>>& GetTexture2DLibraries() { return m_Texture2DLibrary; }
	private:
		//2D纹理库
		std::unordered_map<std::string, Ref<Texture2D>> m_Texture2DLibrary;
	};
}



