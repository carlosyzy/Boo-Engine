#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
#include <functional>
#include <mutex>
// #include "../global/event.h"
// #include "../input/input-mgr.h"

class WindowMgr
{
private:
	WindowMgr();
	~WindowMgr();
	WindowMgr(const WindowMgr&) = delete;            // 禁用拷贝构造
	WindowMgr& operator=(const WindowMgr&) = delete; // 禁用赋值操作符

	GLFWwindow* _window;
	int _width;
	int _height;

public:
	static WindowMgr* getInstance();
	void init();
	GLFWwindow* getWindow();
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void windowSizeCallback(GLFWwindow* window, int width, int height);
	void onCursorPos(double xpos, double ypos);
	void onMouseButton(int button, int action, int mods);
	void onWindowSize();
	void getWindowSize(int& width, int& height);
	void tick();
	void destroy();
};
