#pragma once
#include <string>
#include <functional>
#include <mutex>
#include <map>
#include <variant>  // variant 头文件

#include <typeindex> // 添加type_index支持

using EventValue = std::variant<std::string, int, float, bool>;

class Event
{
private:
    // 使用唯一ID标识监听器
    uint64_t next_id_ = 0;
    struct Listener
    {
        std::function<void(const EventValue&)> callback;
        void *owner;
        uint64_t id; // 唯一标识符

        Listener(std::function<void(const EventValue&)> cb, void *own, uint64_t id)
            : callback(cb), owner(own), id(id) {}
    };
    std::unordered_map<std::string, std::vector<Listener>> _listeners;

public:
    Event();  // 使用default实现
    ~Event(); // 使用default实现

    /**
     * @brief 注册事件监听器
     * @param eventName 事件名称
     * @param callback 事件处理函数
     * @param owner 事件处理函数所属类的指针
     * @return uint64_t 监听器ID
     */
    template <typename T, typename Func>
    uint64_t on(const std::string eventName, Func func, T *instance)
    {
        uint64_t id = this->next_id_++;
        auto callback = [instance, func](const EventValue& value)
        {
            (instance->*func)(value);
        };
        this->_listeners[eventName].emplace_back(callback, static_cast<void *>(instance), id);
        return id;
    }

    /**
     * @brief 移除指定ID的事件监听器
     * @param eventName 事件名称
     * @param id 监听器ID
     */
    void off(uint64_t id)
    {
        for (auto it = this->_listeners.begin(); it != this->_listeners.end(); ++it)
        {
            auto &listeners = it->second;
            for (auto listenerIt = listeners.begin(); listenerIt != listeners.end(); ++listenerIt)
            {
                if (listenerIt->id == id)
                {
                    listeners.erase(listenerIt);
                    return;
                }
            }
        }
    }
    /**
     * @brief 移除指定类的所有事件监听器
     * @param eventName 事件名称
     * @param instance 事件处理函数所属类的指针
     */
    template <typename T>
    void offTarget(T *instance)
    {
        for (auto it = this->_listeners.begin(); it != this->_listeners.end(); ++it)
        {
            auto &listeners = it->second;
            for (auto listenerIt = listeners.begin(); listenerIt != listeners.end(); ++listenerIt)
            {
                if (listenerIt->owner == static_cast<void *>(instance))
                {
                    listeners.erase(listenerIt);
                }
            }
        }
    }

    /**
     * @brief 触发事件
     * @param eventName 事件名称
     */
    template <typename... Args>
    void emit(const std::string eventName, const EventValue& value)
    {
        auto it = this->_listeners.find(eventName);
        if (it != this->_listeners.end())
        {
            for (auto &listener : it->second)
            {
                listener.callback(value);
            }
        }
    }
};
// Event() = default;                        // 使用default实现
// ~Event() = default;                       // 使用default实现
// Event(const Event &) = delete;            // 禁用拷贝构造
// Event &operator=(const Event &) = delete; // 禁用赋值操作符
// /**
//  * 默认事件
//  */
// struct DefaultEvent
// {
//     std::string eventName;
// };
// /**
//  * 触摸事件
//  */
// struct TouchStartEvent
// {
//     std::string eventName;
// };
// struct TouchMoveEvent
// {
//     std::string eventName;
// };
// struct TouchEndEvent
// {
//     std::string eventName;
// };

// struct EventHandler
// {
//     std::string classPtr; // 类指针
//     std::function<void(const void *)> callback;
// };
// std::mutex mutex;
// std::map<std::type_index, std::map<std::string, std::vector<std::function<void(const void *)>>>> _handlers;

// template <typename EventType>
//     using EventHandlerFunc = std::function<void(const EventType &)>;

//     /**
//      * 注册事件监听器
//      * @param eventName 事件名称
//      * @param className 事件处理函数
//      * @param listener 注册类的指针
//      */
//     template <typename EventType, typename T>
//     void on(const std::string &eventName, EventHandlerFunc<EventType> listener, const T *instance);

//     /**
//      * 触发事件
//      * @param eventName 事件名称
//      * @param event 事件数据
//      */
//     template <typename EventType, typename T>
//     void emit(const std::string &eventName, const EventType &event);

//     /**
//      * 移除指定事件
//      */
//     template <typename EventType, typename T>
//     void removeListener(const std::string &eventName, EventHandlerFunc<EventType> listener = nullptr, const T *instance);
//     /**
//      * 移除特定类的所有事件监听
//      * @param className 要移除的类名
//      */
//     template <typename T>
//     void removeAll(const T *instance);