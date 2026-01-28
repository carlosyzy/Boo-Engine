#pragma once
#include <iostream>
#include <string>
#include <filesystem>

class Window;
class Engine;
class EditorBoot;

class Editor
{
private:
    Window *_window;
    Engine *_engine;
    EditorBoot *_boot;

    long long _deltaTime;
    int _frameRate;

    void _initWindow();
    void _initEngine();
    void _initEditorMain();

    void _tick();
public:
    Editor();

    /**
     * @brief 初始化
     */
    void init();
    /**
     * @brief
     */
    void run();

    void destroy();
    ~Editor();
};
