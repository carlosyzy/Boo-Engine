
#include "window-mgr.h"

const int MIN_WIDTH = 640;
const int MIN_HEIGHT = 360;

WindowMgr::WindowMgr() : _windowDirty(0), _width(0), _height(0),_window(nullptr)
{
}
WindowMgr::~WindowMgr()
{
}
WindowMgr* WindowMgr::getInstance()
{
	static WindowMgr instance;
	return &instance;
}

void WindowMgr::init()
{
	if (!glfwInit())
	{
		return;
	}

	// 获取显示器的视频模式
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	// 获取显示器的视频模式
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	float width = mode->width * 0.7f;
	width = width < MIN_WIDTH ? MIN_WIDTH : width;
	float height = mode->height * 0.7f;
	height = height < MIN_HEIGHT ? MIN_HEIGHT : height;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//// const int C_GLFW_TRANSPARENT_FRAMEBUFFER = 131082;
	//// glfwWindowHint(C_GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

	// 创建窗口
	this->_window = glfwCreateWindow(width, height, "", nullptr, nullptr);
	if (!this->_window)
	{
		glfwTerminate();
		return;
	}
	glfwSetWindowSizeLimits(this->_window, MIN_WIDTH, MIN_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwSetWindowUserPointer(this->_window, this);
	glfwSetCursorPosCallback(this->_window, WindowMgr::cursorPosCallback);
	glfwSetMouseButtonCallback(this->_window, WindowMgr::mouseButtonCallback);
	glfwSetWindowSizeCallback(this->_window, WindowMgr::windowSizeCallback);
	this->onWindowSize();
}
GLFWwindow* WindowMgr::getWindow()
{
	return this->_window;
}
void WindowMgr::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	WindowMgr* manager = static_cast<WindowMgr*>(glfwGetWindowUserPointer(window));
	std::cout << "aaa:" << manager << std::endl;
	if (manager != nullptr) // 检查非空
	{
		manager->onCursorPos(xpos, ypos); // 调用非静态处理函数
	}
}
void WindowMgr::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	WindowMgr* manager = static_cast<WindowMgr*>(glfwGetWindowUserPointer(window));
	//if (manager != nullptr)
	//{
	//	manager->onMouseButton(button, action, mods); // 调用非静态处理函数
	//}
}
void WindowMgr::windowSizeCallback(GLFWwindow* window, int width, int height)
{
	WindowMgr* manager = static_cast<WindowMgr*>(glfwGetWindowUserPointer(window));
	//if (manager != nullptr)
	//{
	//	manager->onWindowSize(); // 调用非静态处理函数
	//}
}

void WindowMgr::onCursorPos(double xpos, double ypos)
{
	// InputMgr::getInstance()->onCursorPos(xpos, ypos);
	std::cout << "WindowMgr::onCursorPos:" << std::endl;
}
void WindowMgr::onMouseButton(int button, int action, int mods)
{
	std::cout << "WindowMgr::onMouseButton:" << button << "   " << action << "   " << mods << std::endl;
	// InputMgr::getInstance()->onMouseButton(button, action, mods);
}
void WindowMgr::onWindowSize()
{
	////  交换链扩展应该使用帧缓冲大小，而不是窗口大小
	//int width = 0;
	//int height = 0;
	//glfwGetWindowSize(this->_window, &width, &height);
	//std::lock_guard<std::mutex> lock(this->_windowMutex);
	//this->_width = width;
	//this->_height = height;
	//this->_windowDirty++;
	////std::cout << "WindowMgr::onWindowSize:" << width << "   " << height << std::endl;
}
//void WindowMgr::getWindowSize(int& width, int& height)
//{
//	std::lock_guard<std::mutex> lock(this->_windowMutex);
//	width = this->_width;
//	height = this->_height;
//}
//void WindowMgr::tick()
//{
//	while (!glfwWindowShouldClose(this->_window))
//	{
//		glfwPollEvents();
//	}
//}
//
//// 线程安全的获取方法
// int WindowMgr::getWindowDirty()
//{
//	std::lock_guard<std::mutex> lock(this->_windowMutex);
//	return this->_windowDirty;
// }
//
// void WindowMgr::setWindowDirty(int dirty)
//{
//	std::lock_guard<std::mutex> lock(this->_windowMutex);
//	this->_windowDirty = dirty;
// }
// void WindowMgr::destroy()
// {
//	 glfwDestroyWindow(this->_window);
//	 glfwTerminate();
// }


