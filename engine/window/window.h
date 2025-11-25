#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
#include <functional>
#include <mutex>
#include <atomic>
class Window
{
private:
	GLFWwindow* _window;
	int _width;
	int _height;
public:
	Window();
	void init();
	const int getWidth()
	{
		return this->_width;
	}
	const int getHeight()
	{
		return this->_height;
	}
	GLFWwindow* getWindow();
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void windowSizeCallback(GLFWwindow* window, int width, int height);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		
	void onCursorPos(double xpos, double ypos);
	void onMouseButton(int button, int action, int mods);
	void onKey(int key, int scancode, int action, int mods);
	void onWindowSize();
	bool isRunning();
	void tick();
	void destroy();
	~Window();
};
