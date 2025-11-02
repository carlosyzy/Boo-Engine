#include "event.h"

Event::Event()
{
}
Event::~Event()
{
}
// template <typename T, typename Func>
// uint64_t Event::on(const std::string eventName, Func func, T *instance)
// {
//     uint64_t id = this->next_id_++;
//     auto callback = [instance, func]()
//     {
//         (instance->*func)();
//     };
//     this->_listeners[eventName].emplace_back(callback, static_cast<void*>(instance), id);
//     return id;
// }
// /**
//  * @brief 移除指定ID的事件监听器
//  * @param eventName 事件名称
//  * @param id 监听器ID
//  */
// void Event::off(uint64_t id)
// {
//     for (auto it = this->_listeners.begin(); it != this->_listeners.end(); ++it)
//     {
//         auto &listeners = it->second;
//         for (auto listenerIt = listeners.begin(); listenerIt != listeners.end(); ++listenerIt)
//         {
//             if (listenerIt->id == id)
//             {
//                 listeners.erase(listenerIt);
//                 return;
//             }
//         }
//     }
// }

// /**
//  * @brief 移除指定类的所有事件监听器
//  * @param eventName 事件名称
//  * @param instance 事件处理函数所属类的指针
//  */
// template<typename T>
// void Event::offTarget(T* instance)
// {
//     for (auto it = this->_listeners.begin(); it != this->_listeners.end(); ++it)
//     {
//         auto &listeners = it->second;
//         for (auto listenerIt = listeners.begin(); listenerIt != listeners.end(); ++listenerIt)
//         {
//             if (listenerIt->owner == static_cast<void*>(instance))
//             {
//                 listeners.erase(listenerIt);
//             }
//         }
//     }
// }
// /**
//  * @brief 触发事件
//  * @param eventName 事件名称
//  * @param args 事件参数
//  */
// template<typename... Args>
// void Event::emit(const std::string eventName, Args... args)
// {
//     auto it = this->_listeners.find(eventName);
//     if (it != this->_listeners.end())
//     {
//         for (auto &listener : it->second)
//         {
//             if (listener.callback) {
//                 listener.callback(args...);
//             }
//         }
//     }
// }



// Event &Event::getInstance()
// {
//     static Event instance;
//     return instance;
// }
// void Event::on(Callback callback)
// {
//     this->_listeners.push_back(callback);
// }
// void Event::emit()
// {
//     for (auto &listener : this->_listeners)
//     {
//         listener();
//     }

// }

// template< typename EventType,typename T>
// void Event::on(const std::string &eventName, EventHandlerFunc<EventType> listener, const T *instance){
//     std::lock_guard<std::mutex> lock(mutex);

//     auto &typeHandlers = handlers[typeid(EventType)];
//     if (typeHandlers.find(eventName) == typeHandlers.end())
//     {
//         typeHandlers[eventName] = std::vector<EventHandler>();
//     }
//     auto &handlers = typeHandlers[eventName];
//     handlers.emplace_back(
//         EventHandler{
//             instance,
//             listener});

// }

// template <typename EventType>
// void Event::on(const std::string &eventName, const std::string &className, EventHandlerFunc<EventType> listener)
// {
//     std::lock_guard<std::mutex> lock(mutex);

//     handlers[typeid(EventType)][eventName].emplace_back(
//         EventHandler{
//             className,
//             [listener](const void *event)
//             {
//                 listener(*static_cast<const EventType *>(event));
//             }});
// }

// template <typename EventType>
// void Event::emit(const std::string &eventName, const EventType &event)
// {
//     std::vector<EventHandler> listeners;
//     {
//         std::lock_guard<std::mutex> lock(mutex);
//         auto typeIt = handlers.find(typeid(EventType));
//         if (typeIt != handlers.end())
//         {
//             auto nameIt = typeIt->second.find(eventName);
//             if (nameIt != typeIt->second.end())
//             {
//                 listeners = nameIt->second;
//             }
//         }
//     }

//     for (auto &handler : listeners)
//     {
//         handler.callback(&event);
//     }
// }
// void Event::remove(const std::string &className)
// {
//     std::lock_guard<std::mutex> lock(mutex);

//     for (auto &typePair : handlers)
//     {
//         for (auto &namePair : typePair.second)
//         {
//             auto &handlers = namePair.second;
//             handlers.erase(
//                 std::remove_if(handlers.begin(), handlers.end(),
//                                [&className](const EventHandler &h)
//                                {
//                                    return h.className == className;
//                                }),
//                 handlers.end());
//         }
//     }
// }

// // 自定义事件类型
// struct MyEvent {
//     int value;
//     std::string message;
// };

// int main() {
//     // 订阅事件
//     Event::getInstance().on<MyEvent>([](const MyEvent& e) {
//         std::cout << "Received: " << e.value << ", " << e.message << std::endl;
//     });

//     // 发布事件
//     Event::getInstance().emit(MyEvent{42, "Hello World"});

//     return 0;
// }