
#include "window-mgr.h"

const int MIN_WIDTH = 640;
const int MIN_HEIGHT = 360;



WindowMgr::WindowMgr() : _width(0), _height(0), _window(nullptr)
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

	this->_window_width.store(width, std::memory_order_relaxed);
	this->_window_height.store(height, std::memory_order_relaxed);
	this->_size_changed.store(false, std::memory_order_release); // 通知子线程
}
GLFWwindow* WindowMgr::getWindow()
{
	return this->_window;
}
void WindowMgr::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	void* userPointer = glfwGetWindowUserPointer(window);
	if (userPointer == nullptr) {
		return;
	}
	WindowMgr* manager = static_cast<WindowMgr*>(userPointer);
	if (manager == nullptr) {
		return;
	}
	manager->onCursorPos(xpos, ypos); 

}
void WindowMgr::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	void* userPointer = glfwGetWindowUserPointer(window);
	if (userPointer == nullptr) {
		return;
	}
	WindowMgr* manager = static_cast<WindowMgr*>(userPointer);
	if (manager == nullptr) 
	{
		return;
	}
	manager->onMouseButton(button, action, mods);
}
void WindowMgr::windowSizeCallback(GLFWwindow* window, int width, int height)
{
	void* userPointer = glfwGetWindowUserPointer(window);
	if (userPointer == nullptr) {
		return;
	}
	WindowMgr* manager = static_cast<WindowMgr*>(userPointer);
	if (manager == nullptr) // 检查非空
	{
		return;
	}
	manager->onWindowSize();

};

void WindowMgr::onCursorPos(double xpos, double ypos)
{
	// InputMgr::getInstance()->onCursorPos(xpos, ypos);
	std::cout << "WindowMgr::onCursorPos:" << std::endl;
};
void WindowMgr::onMouseButton(int button, int action, int mods)
{
	std::cout << "WindowMgr::onMouseButton:" << button << "   " << action << "   " << mods << std::endl;
	// InputMgr::getInstance()->onMouseButton(button, action, mods);
};
void WindowMgr::onWindowSize()
{
	int width = 0;
	int height = 0;
	glfwGetWindowSize(this->_window, &width, &height);
	this->_width = width;
	this->_height = height;
	this->_window_width.store(width, std::memory_order_relaxed);
	this->_window_height.store(height, std::memory_order_relaxed);
	this->_size_changed.store(true, std::memory_order_release); // 通知子线程
};
bool WindowMgr::getWindowSize(int& width, int& height)
{
	if (this->_size_changed.load(std::memory_order_acquire)) {
		width = this->_window_width.load(std::memory_order_relaxed);
		height = this->_window_height.load(std::memory_order_relaxed);
		this->_size_changed.store(false, std::memory_order_release);
		return true;
	}
	return false;
}
void WindowMgr::tick()
{
	while (!glfwWindowShouldClose(this->_window))
	{
		glfwPollEvents();
	}
}
 void WindowMgr::destroy()
 {
	 glfwDestroyWindow(this->_window);
	 glfwTerminate();
 }
