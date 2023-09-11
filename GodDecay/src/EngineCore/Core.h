#pragma once

namespace GodDecay 
{
	#ifdef GD_PLATFORM_WINDOWS
		#ifdef GD_BUILD_DLL
			#define GODDECAY_API __declspec(dllexport)
		#else
			#define GODDECAY_API __declspec(dllimport)
		#endif // GD_BUILD_DLL
	#else
		#error GodDecay only supports Windows
	#endif // GD_PLATFORM_WINDOWS
}