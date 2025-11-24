#pragma once
#include <string>
#include <iostream>
#include <functional>
#include <unordered_map>


class Event;
class Scene;
class Node;
class Component;
class AssetsManager;
class FreetypeMgr;
class Input;



struct View
{
    bool isFlag;
    int width = 1280;
    int height = 720;
};
struct ScheduleInfo
{
    std::function<void()> func;
    void *instance;
    float interval;
    float time;
    bool isOnce;
    ScheduleInfo()
        : func(), instance(nullptr), interval(0.0f), time(0.0f), isOnce(false)
    {
    }
    ScheduleInfo(std::function<void()> f, void *i, float in, float t, bool once)
    {
        this->func = f;
        this->instance = i;
        this->interval = in;
        this->time = t;
        this->isOnce = once;
    }
};

class Game
{
private:
    // 调度器相关
    uint64_t _scheduleNextID_ = 0;
    std::unordered_map<int, ScheduleInfo> _schedules;
    // 组件创建相关
    std::unordered_map<std::string, std::function<Component *(Node *, std::string)>> _creatorComponentMap;
    // 组件清理相关
    std::vector<Component *> _compClearCaches;
    // 节点清理相关
    std::vector<Node *> _nodeClearCaches;


    float _logicTime = 0.0f;
    float _renderTime = 0.0f;

    /**
     * @brief 事件系统
     */
    Event *_event;
    /**
     * @brief 视图系统
     */
    View *_view;
    /**
     * @brief 资产系统
     */
    AssetsManager *_assetsManager;
    /**
     * @brief 字体系统
     */
    FreetypeMgr *_fontMgr;
    /**
     * @brief 输入系统
     */
    Input *_input;
    

    /**
     * @brief 当前场景
     */
    Scene *_curScene;

    /**
     * @brief 初始化图形库
     */
    void _initGFX();
    /**
     * @brief 初始化事件系统
     */
    void _initEvent();
    /**
     * @brief 初始化输入系统
     */
    void _initInput();
    /**
     * @brief 初始化视图系统
     */
    void _initView();
    /**
     * @brief 初始化字体系统
     */
    void _initFont();
    /**
     * @brief 初始化组件系统
     */
    void _initComponents();
    /**
     * @brief 初始化资产系统
     */
    void _initAssets();
    /**
     * @brief 初始化透明度系统
     */
    void _initAlpha();

    void _update(float dt);
    void _lateUpdate(float dt);
    void _render(float dt);
    void _clear();
    void _updateSchedules(float dt);
    void _updateClearCaches();

public:
    Game();
    ~Game();
    /**
     * @brief 初始化游戏
     */
    void init();
    void setView(const int width, const int height);
    View *view()
    {
        return this->_view;
    }
    Event *event()
    {
        return this->_event;
    }
    AssetsManager *assetsManager()
    {
        return this->_assetsManager;
    }
    FreetypeMgr *fontMgr()
    {
        return this->_fontMgr;
    }
    Input *input()
    {
        return this->_input;
    }

    Scene *getScene()
    {
        return this->_curScene;
    }
    void openScene(Scene *scene);
    void destroyScene();


    // 注册组件类
    template <typename T = Component>
    void registerComponentClass(const std::string &className)
    {
        static_assert(std::is_base_of<Component, T>::value,
                      "T must be derived from Component");
        this->_creatorComponentMap[className] = [](Node *node, std::string uuid) -> Component *
        {
            return new T(node, uuid);
        };
    }
    Component *createComponent(const std::string &className, Node *node, std::string uuid);

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
        this->_schedules[id] = ScheduleInfo(callback, instance, interval, 0.0f, false);
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
        // this->_schedules[id] = ScheduleInfo(callback, instance, interval, 0.0f, true);
        ScheduleInfo scheduleInfo(callback, instance, interval, 0.0f, true);
        this->_schedules[id] = scheduleInfo;
        return id;
    }
    void unschedule(int scheduleID);

   /* Scene *openScene(std::string sceneName);*/

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
    void tick(float dt);
};
