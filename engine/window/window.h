#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
#include <functional>
#include <mutex>
#include <atomic>
class Engine;
class Window
{
private:
	Engine *_engine;
	GLFWwindow* _window;
	int _width;
	int _height;
public:
	Window(Engine *engine);
	void init();
	GLFWwindow* getWindow();
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void windowSizeCallback(GLFWwindow* window, int width, int height);
	void onCursorPos(double xpos, double ypos);
	void onMouseButton(int button, int action, int mods);
	void onWindowSize();
	
	bool isRunning();
	void tick();
	void destroy();
	~Window();
};
