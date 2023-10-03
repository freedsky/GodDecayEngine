#include "gdpch.h"
#include "WindowsInput.h"

#include <GLFW/glfw3.h>

namespace GodDecay
{
	Input* Input::m_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(uint32_t keycode) const
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());

		int status = glfwGetKey(window, keycode);
		return status == GLFW_PRESS || status == GLFW_REPEAT;
	}

	bool WindowsInput::IsMosueButtonPressedImpl(uint32_t button) const
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());

		int status = glfwGetMouseButton(window, button);
		return status == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMosuePositionImpl() const
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
		int width, height;
		glfwGetWindowSize(window, &width, &height);

		//限制坐标超出屏幕
		double Xpos, Ypos;
		glfwGetCursorPos(window, &Xpos, &Ypos);
		if(Xpos >= 0 && Xpos <= width && Ypos >= 0 && Ypos <= height)
			return { (float)Xpos,(float)Ypos };
		return{ (float)-1,(float)-1 };
	}

	float WindowsInput::GetMouseXImpl() const
	{
		std::pair<float, float> position = GetMosuePosition();
		return position.first;
	}

	float WindowsInput::GetMouseYImpl() const
	{
		std::pair<float, float> position = GetMosuePosition();
		return position.second;
	}

}
