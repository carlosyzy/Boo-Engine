
#pragma once
#include <memory> // 智能指针头文件
#include <thread> // 线程头文件
#include <string>

class Scene;

class Engine
{
private:
    long _deltaTime;
    int _frameRate;
    /**
     * @brief 初始化窗口
     */
    void _initWindow();
    /**
     * @brief 初始Game
     */
    void _initGame();

    /**
     * @brief 初始化图形库
     */
    void _initEditor();

public:
    Engine();
    /**
     * 初始化
     */
    void init();
    /**
     * 主循环
     */
    void tick();
    void update(float dt);

    ~Engine();
};
