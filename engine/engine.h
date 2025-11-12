
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
     * @brief 初始化图形库
     */
    void _initEditor();
    /**
     * @brief 初始Game
     */
    void _initGame();

    bool _isRenderering;
    std::thread _renderThread; // 渲染线程
    void _startRendererThread();
    void _updateRendererLoop();
    void _stopRendererThread();

public:
    Engine();
    /**
     * 初始化
     */
    void init();
    /**
     * 启动
     */
    void launch();
    /**
     * 启动编辑器
     */
    void launchEditor();
    /**
     * 主循环
     */
    void tick();
    void update(float dt);

    ~Engine();
};
