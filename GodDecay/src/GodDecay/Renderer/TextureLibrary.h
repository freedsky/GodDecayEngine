#pragma once
#include "Texture.h"

#include <string>
#include <unordered_map>

/// <summary>
/// textures���ؿ�
/// ÿ����Texture���뵽�ڴ�ʱ���洢�ڿ���
/// 
/// ���ǵ���ͬ���͵�Textrue��ô�죿
/// 
/// </summary>
namespace GodDecay 
{
	class TextureLibrary
	{
	public:
		//����2D����
		void AddTexture2D(const std::string& name, const Ref<Texture2D>& texture);
		void AddTexture2D(const Ref<Texture2D>& texture);

		Ref<Texture2D> LoadTexture2D(const std::string& name, const std::string& path);
		Ref<Texture2D> LoadTexture2D(const std::string& name, uint32_t width, uint32_t height);

		//����key�޸�key��Ӧ��value
		void UpdataTexture2D(const std::string& T_name, Ref<Texture2D> texture);

		Ref<Texture2D> Get(const std::string& name);
	
		bool Exists(const std::string& name) const;

		inline std::unordered_map<std::string, Ref<Texture2D>>& GetTexture2DLibraries() { return m_Texture2DLibrary; }
	private:
		//2D�����
		std::unordered_map<std::string, Ref<Texture2D>> m_Texture2DLibrary;
	};
}



