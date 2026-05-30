#pragma once
#include <string>
#include <functional>
#include <mutex>
#include <map>
#include <variant> // variant 头文件
#include <unordered_map>
#include <vector>
#include <typeindex> // 添加type_index支持

namespace Boo
{

    const std::string EventEngine_Launch = "Launch";

    class Event
    {
    private:
        // 使用唯一ID标识监听器
        uint64_t next_id_ = 0;
        struct Listener
        {
            std::function<void()> callback;
            uint64_t id; // 唯一标识符
            Listener(std::function<void()> cb, uint64_t id)
                : callback(std::move(cb)), id(id) {} // 使用 std::move 移动构造函数
        };
        std::unordered_map<std::string, std::vector<Listener>> _listeners;
        
        // 带整数参数的监听器
        struct ListenerWithInt
        {
            std::function<void(int)> callback;
            uint64_t id;
            ListenerWithInt(std::function<void(int)> cb, uint64_t id)
                : callback(std::move(cb)), id(id) {}
        };
        std::unordered_map<std::string, std::vector<ListenerWithInt>> _listenersWithInt;

    public:
        Event();  // 使用default实现
        ~Event(); // 使用default实现
        uint64_t on(const std::string &eventName, std::function<void()> call)
        {
            uint64_t id = this->next_id_++;
            this->_listeners[eventName].emplace_back(std::move(call), id);
            return id;
        }
        
        // 带整数参数的事件监听
        uint64_t on(const std::string &eventName, std::function<void(int)> call)
        {
            uint64_t id = this->next_id_++;
            this->_listenersWithInt[eventName].emplace_back(std::move(call), id);
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
            // 同时移除带参数的监听器
            for (auto it = this->_listenersWithInt.begin(); it != this->_listenersWithInt.end(); ++it)
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
         * @brief 移除指定事件的所有监听器
         * @param eventName 事件名称
         */
        void off(const std::string &eventName)
        {
            auto it = _listeners.find(eventName);
            if (it != _listeners.end())
            {
                size_t count = it->second.size();
                _listeners.erase(it);
            }
            auto it2 = _listenersWithInt.find(eventName);
            if (it2 != _listenersWithInt.end())
            {
                _listenersWithInt.erase(it2);
            }
        }
        /**
         * @brief 触发事件
         * @param eventName 事件名称
         */
        void emit(const std::string &eventName)
        {
            auto it = this->_listeners.find(eventName);
            if (it != this->_listeners.end())
            {
                for (auto &listener : it->second)
                {
                    listener.callback();
                }
            }
        }
        
        /**
         * @brief 触发带整数参数的事件
         * @param eventName 事件名称
         * @param value 整数值
         */
        void emit(const std::string &eventName, int value)
        {
            auto it = this->_listenersWithInt.find(eventName);
            if (it != this->_listenersWithInt.end())
            {
                for (auto &listener : it->second)
                {
                    listener.callback(value);
                }
            }
        }
    };

} // namespace Boo