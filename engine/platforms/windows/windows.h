#pragma once
#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <functional>
#include <mutex>
#include <atomic>
#include "platforms/platform.h"
#if defined(BOO_PLATFORM_WINDOWS)
#define NOMINMAX // 禁用 Windows 头文件中定义的 min 和 max 宏。
#include <GLFW/glfw3.h>
#include <windows.h>
#endif
class Windows
{
private:
    int _width;
    int _height;
    std::string _assetsRoot = "";
    void _initAssetsRoot();

private:
#if defined(BOO_PLATFORM_WINDOWS)
    GLFWwindow *_window = nullptr;
#endif

public:
    Windows(int width, int height);
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
    ~Windows();

public:
#if defined(BOO_PLATFORM_WINDOWS)
    GLFWwindow *getWindow();
    static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void windowSizeCallback(GLFWwindow *window, int width, int height);
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    void onWindowSize();
#endif
};
