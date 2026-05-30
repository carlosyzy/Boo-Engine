#pragma once
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <unordered_map>
#include <cstdint>
class Windows;
class MacOS;
class Android;
class HarmonyOS;
namespace Boo
{
    class Scene;
    class Camera;
    class Node;
    class Node3D;
    class Node2D;
    class Renderer;
    class Component;

    enum class UIDesignFitMode
    {
        /**
         * 无适配(宽度和高度都不变,常用于全屏游戏)
         */
        None,
        /**
         * 宽适配(宽度不变，高度按比例缩放,超出部分裁剪,常用于竖屏游戏)
         */
        Width,
        /**
         * 高适配(高度不变，宽度按比例缩放,超出部分裁剪,常用于横屏游戏)
         */
        Height,
    };
    struct InitConfig
    {
        int fps;
        int designWidth;
        int designHeight;
        UIDesignFitMode fitMode;
    };
    struct ScheduleInfo
    {
        std::function<void()> func;
        void *instance;
        float interval;
        float time;
        bool isOnce;
        bool clearFlag;
        ScheduleInfo()
            : func(), instance(nullptr), interval(0.0f), time(0.0f), isOnce(false), clearFlag(false)
        {
        }
        ScheduleInfo(std::function<void()> f, void *i, float in, bool once)
        {
            this->func = f;
            this->instance = i;
            this->interval = in;
            this->isOnce = once;
            this->time = 0.0f;
            this->clearFlag = false;
        }
    };

    class Game
    {
    private:
        /**
         * @brief 引擎设置帧率
         */
        int _frameRate;
        /**
         * @brief 实际帧率
         */
        int _fps;
        long long _deltaTime;

        /**
         * @brief 视图是否改变
         */
        bool _viewChanged = false;
        long long _viewChangedTime = 0;
        /**
         * @brief 当前场景
         */
        Scene *_curScene;
    
    private:
        // 平台相关
        Windows *_windows;
        MacOS *_macos;
        Android *_android;
        HarmonyOS *_harmonyos;
    private:
        // 调度器相关
        uint64_t _scheduleNextID_ = 0;
        std::unordered_map<int, ScheduleInfo> _schedules;
    private:
        std::vector<int> _scheduleClearCaches;
        // 组件清理相关
        std::vector<Component *> _compClearCaches;
        // 节点清理相关
        std::vector<Node *> _nodeClearCaches;
    private:
        /**
         * @brief 初始化事件系统
         */
        void _initEvent();
        /**
         * @brief 初始化输入系统
         */
        void _initInput();
        /**
         * @brief 初始化资产系统
         */
        void _initAssets();
        /**
         * @brief 初始化音频系统
         */
        void _initAudio();
        /**
         * @brief 初始化性能分析系统
         */
        void _initProfiler();
        /**
         * @brief 初始化渲染系统
         */
        void _initRenderer();

        /**
         * @brief 初始化透明度系统
         */
        void _initAlpha();

        /**
         * @brief 初始化视图
         * @param uiDesignWidth 设计宽度
         * @param uiDesignHeight 设计高度
         * @param fitMode 适配模式
         * @param width 实际宽度
         * @param height 实际高度
         */
        void _initView(int width, int height, InitConfig &config);

        void _update(float dt);
        void _updateSchedules(float dt);
        void _updateSystem(float dt);
        void _lateUpdate(float dt);
        void _render(float dt);
        void _clear();
        void _updateClearCaches();

    public:
        Game();
        ~Game();
        void init(Windows *windows, InitConfig &config);
        void init(MacOS *macos, InitConfig &config);
        void init(Android *android, InitConfig &config);
        void init(HarmonyOS *harmonyos, InitConfig &config);
        Windows *getWindows();
        MacOS *getMacOS();
        Android *getAndroid();
        HarmonyOS *getHarmonyOS();

        void setFrameRate(int frameRate);
        const int getFrameRate() const;
        const int getFps() const;
        Scene *getScene();
        void openScene(Scene *scene);
        void destroyScene();
        void resizeView(const int width, const int height);
        // typename T: 表示一个类型参数，通常指类的类型
        // typename Func: 表示另一个类型参数，通常指函数类型（函数指针、成员函数指针、函数对象等）
        template <typename T, typename Func>
        int schedule(Func func, T *instance, float interval)
        {
            int id = this->_scheduleNextID_++;
            auto callback = [instance, func]()
            {
                (instance->*func)();
            };
            this->_schedules[id] = ScheduleInfo(callback, instance, interval, false);

            return id;
        }
        template <typename T, typename Func>
        int scheduleOnce(Func func, T *instance, float interval)
        {
            int id = this->_scheduleNextID_++;
            auto callback = [instance, func]()
            {
                (instance->*func)();
            };
            ScheduleInfo scheduleInfo(callback, instance, interval, true);
            this->_schedules[id] = scheduleInfo;
            return id;
        }
        void unschedule(int scheduleID);
        void addCompClearCaches(Component *comp);
        void addNodeClearCaches(Node *node);
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
        /**
         * @brief 更新键盘状态
         *
         * @param key 键盘键值
         * @param scancode 键盘扫描码
         * @param action 键盘操作 0:抬起 1:按下
         * @param mods 键盘修饰键 0:无 1:Shift 2:Ctrl 3:Alt
         */
        void updateKeyState(int key, int scancode, int action, int mods);
        /**
         * @brief 更新滚动状态
         *
         * @param xoffset 滚动X偏移量
         * @param yoffset 滚动Y偏移量
         */
        void updateScroll(double xoffset, double yoffset);
        /**
         * @brief 更新触摸状态
         * @param action 触摸操作 0:抬起 1:按下
         * @param x 触摸X坐标
         * @param y 触摸Y坐标
         */
        void updateTouch(int action, float x, float y);

        void tick();
    };

} // namespace Boo
