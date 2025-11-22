
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
    void run();
    void tick(float dt);
    void updateViewSize(float width, float height);
    /**
     * @brief 更新鼠标状态
     * 
     * @param button 鼠标按钮 0:左键 1:右键 2:中键
     * @param action 鼠标操作 0:抬起 1:按下
     * @param mods 键盘修饰键 0:无 1:Shift 2:Ctrl 3:Alt
     */
    void updateMouseState(int button, int action, int mods);
    /**
     * @brief 更新鼠标位置
     * 
     * @param xpos 鼠标X坐标
     * @param ypos 鼠标Y坐标
     */
    void updateMousePos(double xpos, double ypos);

    ~Engine();
};
