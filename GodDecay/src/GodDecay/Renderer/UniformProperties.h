#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

/// <summary>
/// Shader Uniform 属性组
/// 只能进行暂时的妥协
/// 
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
