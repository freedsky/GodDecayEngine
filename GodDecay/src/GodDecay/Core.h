#pragma once

#include <memory>

/// <summary>
/// 包含了库中的基本宏定义
/// </summary>
#ifdef GD_PLATFORM_WINDOWS
	#ifdef GD_BUILD_DLL
		#define GODDECAY_API __declspec(dllexport)
	#else
		#define GODDECAY_API __declspec(dllimport)
	#endif // GD_BUILD_DLL
#else
	#error GodDecay only supports Windows
#endif // GD_PLATFORM_WINDOWS

#define BIT(x) (1 << x)

/// <summary>
/// 模板化智能指针
/// </summary>
namespace GodDecay
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}