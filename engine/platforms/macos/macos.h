#pragma once
#include <iostream>
#include <functional>
#include <filesystem>
#include <fstream>
#include <vector>
#include <mutex>
#include <atomic>
#include "platforms/platform.h"
#if defined(BOO_PLATFORM_MACOS) // ← 改为 #elif
#include <GLFW/glfw3.h>
#include <limits.h>
#include <mach-o/dyld.h>
#include <unistd.h>
#endif
class MacOS
{
private:
	int _width;
	int _height;
	std::string _assetsRoot = "";
	void _initAssetsRoot();

private:
#if defined(BOO_PLATFORM_MACOS)
	GLFWwindow *_window = nullptr;
#endif

public:
	MacOS(int width, int height);
	const int getWidth();
	const int getHeight();
	std::string getAssetsRoot();
	void getMonitorSize(float &width, float &height);
	void onCursorPos(double xpos, double ypos);
	void onMouseButton(int button, int action, int mods);
	void onKey(int key, int scancode, int action, int mods);
	void onScroll(double xoffset, double yoffset);
	bool isRunning();
	void tick();
	void loadImage(const std::string &path,std::vector<uint8_t> &pixels, int &width, int &height, int &channels);
	void loadText(const std::string &path, std::string &text);
	void loadBinary(const std::string &path, std::vector<uint8_t> &bytes);
	void destroy();
	~MacOS();

public:
#if defined(BOO_PLATFORM_MACOS)
	GLFWwindow *getWindow();
	static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
	static void windowSizeCallback(GLFWwindow *window, int width, int height);
	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
	void onWindowSize();
#endif
};
