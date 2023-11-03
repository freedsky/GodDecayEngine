#include "gdpch.h"
#include "GodDecay/Core/Input.h"
#include "GodDecay/Core/Application.h"

#include <GLFW/glfw3.h>

namespace GodDecay
{
	bool Input::IsKeyPressed(uint32_t keycode) 
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		int status = glfwGetKey(window, keycode);
		return status == GLFW_PRESS || status == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(uint32_t button) 
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		int status = glfwGetMouseButton(window, button);
		return status == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition() 
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int width, height;
		glfwGetWindowSize(window, &width, &height);

		//限制坐标超出屏幕
		double Xpos, Ypos;
		glfwGetCursorPos(window, &Xpos, &Ypos);
		if(Xpos >= 0 && Xpos <= width && Ypos >= 0 && Ypos <= height)
			return { (float)Xpos,(float)Ypos };
		return{ (float)-1,(float)-1 };
	}

	float Input::GetMouseX() 
	{
		std::pair<float, float> position = GetMousePosition();
		return position.first;
	}

	float Input::GetMouseY() 
	{
		std::pair<float, float> position = GetMousePosition();
		return position.second;
	}

}
