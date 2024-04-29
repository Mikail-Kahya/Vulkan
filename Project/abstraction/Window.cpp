#include "Window.h"

#include "vulkan/VulkanUtils.h"

using namespace mk;

Window::Window(int width, int height, std::string name)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_WindowPtr = glfwCreateWindow(width, height, std::move(name).c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(m_WindowPtr, this);
	glfwSetFramebufferSizeCallback(m_WindowPtr, &FrameBufferResizeCallback);
}

Window::~Window()
{
	glfwDestroyWindow(m_WindowPtr);
	glfwTerminate();
}

GLFWwindow* Window::GetWindow() const
{
	return m_WindowPtr;
}

bool Window::ShouldClose() const
{
	return glfwWindowShouldClose(m_WindowPtr);
}
