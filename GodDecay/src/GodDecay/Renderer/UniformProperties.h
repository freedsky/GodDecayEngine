#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

/// <summary>
/// Shader Uniform 属性组
/// 只能进行暂时的妥协
/// 
/// 增加更新属性值的方法
/// </summary>
namespace GodDecay 
{
	class UniformProperties 
	{
	public:
		UniformProperties() = default;
		~UniformProperties() = default;

		void AddProperties(std::string& properties, int value) { Int[properties] = value; }
		void AddProperties(std::string properties, float value) { Float[properties] = value; }
		void AddProperties(std::string properties, bool value) { Bool[properties] = value; }
		void AddProperties(std::string properties, glm::vec2 value) { Vec2[properties] = value; }
		void AddProperties(std::string properties, glm::vec3 value) { Vec3[properties] = value; }
		void AddProperties(std::string properties, glm::vec4 value) { Vec4[properties] = value; }
		void AddProperties(std::string properties, glm::mat3 value) { Mat3[properties] = value; }
		void AddProperties(std::string properties, glm::mat4 value) { Mat4[properties] = value; }

		std::unordered_map<std::string, int>& GetInt() { return Int; }
		std::unordered_map<std::string, float>& GetFloat() { return Float; }
		std::unordered_map<std::string, bool>& GetBool() { return Bool; }
		std::unordered_map<std::string, glm::vec2>& GetVec2() { return Vec2; }
		std::unordered_map<std::string, glm::vec3>& GetVec3() { return Vec3; }
		std::unordered_map<std::string, glm::vec4>& GetVec4() { return Vec4; }
		std::unordered_map<std::string, glm::mat3>& GetMat3() { return Mat3; }
		std::unordered_map<std::string, glm::mat4>& GetMat4() { return Mat4; }

		//更新属性值
		void UpdateInt(const std::string& propertity, int value)
		{
			if (Int.find(propertity) != Int.end()) 
				Int[propertity] = value;
			else
				GD_ENGINE_ERROR("This UniformPerprotity Value Is Not Find");
		}
		void UpdateFloat(const std::string& propertity, float value) 
		{
			if (Float.find(propertity) != Float.end())
				Float[propertity] = value;
			else
				GD_ENGINE_ERROR("This UniformPerprotity Value Is Not Find");
		}
		void UpdateBool(const std::string& propertity, bool value)
		{
			if (Bool.find(propertity) != Bool.end())
				Bool[propertity] = value;
			else
				GD_ENGINE_ERROR("This UniformPerprotity Value Is Not Find");
		}
		void UpdateVec2(const std::string& propertity, glm::vec2& value)
		{
			if (Vec2.find(propertity) != Vec2.end())
				Vec2[propertity] = value;
			else
				GD_ENGINE_ERROR("This UniformPerprotity Value Is Not Find");
		}
		void UpdateVec3(const std::string& propertity, glm::vec3& value)
		{
			if (Vec3.find(propertity) != Vec3.end())
				Vec3[propertity] = value;
			else
				GD_ENGINE_ERROR("This UniformPerprotity Value Is Not Find");
		}
		void UpdateVec4(const std::string& propertity, glm::vec4& value)
		{
			if (Vec4.find(propertity) != Vec4.end())
				Vec4[propertity] = value;
			else
				GD_ENGINE_ERROR("This UniformPerprotity Value Is Not Find");
		}
		void UpdateMat3(const std::string& propertity, glm::mat3& value)
		{
			if (Mat3.find(propertity) != Mat3.end())
				Mat3[propertity] = value;
			else
				GD_ENGINE_ERROR("This UniformPerprotity Value Is Not Find");
		}
		void UpdateMat4(const std::string& propertity, glm::mat4& value)
		{
			if (Mat4.find(propertity) != Mat4.end())
				Mat4[propertity] = value;
			else
				GD_ENGINE_ERROR("This UniformPerprotity Value Is Not Find");
		}

	private:
		std::unordered_map<std::string, int> Int;
		std::unordered_map<std::string, float> Float;
		std::unordered_map<std::string, bool> Bool;
		std::unordered_map<std::string, glm::vec2> Vec2;
		std::unordered_map<std::string, glm::vec3> Vec3;
		std::unordered_map<std::string, glm::vec4> Vec4;
		std::unordered_map<std::string, glm::mat3> Mat3;
		std::unordered_map<std::string, glm::mat4> Mat4;
	};
}
