
#include "window.h"
#include "../boo.h"

const int MIN_WIDTH = 640;
const int MIN_HEIGHT = 360;

Window::Window() : _width(0), _height(0), _window(nullptr)
{
}

void Window::init()
{
	if (!glfwInit())
	{
		return;
	}
	// 获取显示器的视频模式
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	// 获取显示器的视频模式
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	float width = mode->width * 0.7f;
	width = width < MIN_WIDTH ? MIN_WIDTH : width;
	float height = mode->height * 0.7f;
	height = height < MIN_HEIGHT ? MIN_HEIGHT : height;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// 创建窗口
	this->_window = glfwCreateWindow(width, height, "", nullptr, nullptr);
	if (!this->_window)
	{
		glfwTerminate();
		return;
	}
	glfwSetWindowSizeLimits(this->_window, MIN_WIDTH, MIN_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwSetWindowUserPointer(this->_window, this);
	glfwSetCursorPosCallback(this->_window, Window::cursorPosCallback);
	glfwSetMouseButtonCallback(this->_window, Window::mouseButtonCallback);
	glfwSetKeyCallback(this->_window, Window::keyCallback);
	glfwSetWindowSizeCallback(this->_window, Window::windowSizeCallback);
	this->onWindowSize();
}
GLFWwindow *Window::getWindow()
{
	return this->_window;
}
void Window::cursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
	void *userPointer = glfwGetWindowUserPointer(window);
	if (userPointer == nullptr)
	{
		return;
	}
	Window *manager = static_cast<Window *>(userPointer);
	if (manager == nullptr)
	{
		return;
	}
	manager->onCursorPos(xpos, ypos);
}
void Window::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	void *userPointer = glfwGetWindowUserPointer(window);
	if (userPointer == nullptr)
	{
		return;
	}
	Window *manager = static_cast<Window *>(userPointer);
	if (manager == nullptr)
	{
		return;
	}
	manager->onMouseButton(button, action, mods);
}
void Window::windowSizeCallback(GLFWwindow *window, int width, int height)
{
	void *userPointer = glfwGetWindowUserPointer(window);
	if (userPointer == nullptr)
	{
		return;
	}
	Window *manager = static_cast<Window *>(userPointer);
	if (manager == nullptr) // 检查非空
	{
		return;
	}
	manager->onWindowSize();
};
void Window::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	void *userPointer = glfwGetWindowUserPointer(window);
	if (userPointer == nullptr)
	{
		return;
	}
	Window *manager = static_cast<Window *>(userPointer);
	if (manager == nullptr) // 检查非空
	{
		return;
	}
	manager->onKey(key, scancode, action, mods);
}

void Window::onCursorPos(double xpos, double ypos)
{
	if (Boo::game == nullptr)
		return;
	// 通知引擎更新鼠标位置
	Boo::game->updateMousePos(xpos, ypos);
};
void Window::onMouseButton(int button, int action, int mods)
{
	if (Boo::game == nullptr)
		return;
	// 通知引擎更新鼠标状态
	Boo::game->updateMouseState(button, action, mods);
};
void Window::onKey(int key, int scancode, int action, int mods)
{
	if (Boo::game == nullptr)
		return;
	// 通知引擎更新键盘状态
	Boo::game->updateKeyState(key, scancode, action, mods);
}
void Window::onWindowSize()
{
	glfwGetWindowSize(this->_window, &this->_width, &this->_height);
	if (Boo::game == nullptr)
		return;
	Boo::game->setView(this->_width, this->_height);
};
bool Window::isRunning()
{
	return !glfwWindowShouldClose(this->_window);
}
void Window::tick()
{
	glfwPollEvents();
}

void Window::destroy()
{
	glfwDestroyWindow(this->_window);
	glfwTerminate();
}

Window::~Window()
{
}