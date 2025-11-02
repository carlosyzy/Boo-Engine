#pragma once
#include <string>
#include <functional>
#include <unordered_map>



class Component;
class Scene;
class ComponentFactory;
class Event;

struct View
{
    int width = 1280;
    int height = 720;
};

class Game
{
private:
    Game();
    ~Game();
    Game(const Game &) = delete;            // 禁用拷贝构造
    Game &operator=(const Game &) = delete; // 禁用赋值操作符
    Event *_event;
    ComponentFactory *_componentFactory = nullptr;
    View _view;
    
    // 场景
    Scene *_curScene;

    void _initEvent();
    void _initInput();
    void _initFont();
    void _initComponents();

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

    Scene *openScene(std::string sceneName);
    void update(float dt);
};
