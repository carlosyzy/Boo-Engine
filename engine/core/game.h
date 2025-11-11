#pragma once
#include <string>
#include <iostream>
#include <functional>
#include <unordered_map>

class Component;
class Scene;
class ComponentFactory;
class Event;
class AssetsManager;
class Node;

struct View
{
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
    Game();
    ~Game();
    Game(const Game &) = delete;            // 禁用拷贝构造
    Game &operator=(const Game &) = delete; // 禁用赋值操作符

    std::vector<Component *> _compClearCaches;
    std::vector<Node *> _nodeClearCaches;

    Event *_event;
    ComponentFactory *_componentFactory = nullptr;
    View _view;
    Scene *_curScene;
    AssetsManager *_assetsManager;

    uint64_t _scheduleNextID_ = 0;
    std::unordered_map<int, ScheduleInfo> _schedules;

    void _initEvent();
    void _initInput();
    void _initFont();
    void _initComponents();
    void _initAssets();
    void _initAlpha();

    void _updateSchedules(float dt);
    void _updateClearCaches();

public:
    static Game *getInstance();
    void init();

    void setView(int width, int height);
    View &view()
    {
        return this->_view;
    }
    Event *event()
    {
        return this->_event;
    }
    ComponentFactory *componentFactory()
    {
        return this->_componentFactory;
    };
    Scene *getScene()
    {
        return this->_curScene;
    }
    AssetsManager *assetsManager()
    {
        return this->_assetsManager;
    }
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
        std::cout << "scheduleOnce: " << id << std::endl;
        auto callback = [instance, func]()
        {
            (instance->*func)();
        };
        // this->_schedules[id] = ScheduleInfo(callback, instance, interval, 0.0f, true);
        ScheduleInfo scheduleInfo(callback, instance, interval, 0.0f, true);
        this->_schedules[id] = scheduleInfo;
        std::cout << "scheduleOnce: " << id << " interval2: " << interval << std::endl;
        return id;
    }
    void unschedule(int scheduleID);

    Scene *openScene(std::string sceneName);

    void addCompClearCaches(Component* comp);
    void addNodeClearCaches(Node* node);

    void update(float dt);
};
