//#pragma once
//#include <string>
//#include <functional>
//#include <map>
//
//#include "../../engine/core/game.h"
//#include "../../engine/core/global/event.h"
//
//enum class IpcEvent
//{
//    FOCUS,       // 焦点事件
//    SELECT_NODE, // 选中节点
//    SELECT_ASSETS,   // 选中资产
//    UPDATE_HIERARCHY_ROOT,    // 更新层级管理器更节点
//
//};
//
//class EditorIpc
//{
//private:
//    EditorIpc();
//    ~EditorIpc();
//    EditorIpc(const EditorIpc &) = delete;            // 禁用拷贝构造
//    EditorIpc &operator=(const EditorIpc &) = delete; // 禁用赋值操作符
//
//    const std::unordered_map<IpcEvent, std::string> toStringMap = {
//        {IpcEvent::FOCUS, "FOCUS"},
//        {IpcEvent::SELECT_NODE, "SELECT_NODE"},
//        {IpcEvent::SELECT_ASSETS, "SELECT_ASSETS"},
//        {IpcEvent::UPDATE_HIERARCHY_ROOT, "UPDATE_HIERARCHY_ROOT"},
//    };
//
//public:
//    static EditorIpc *getInstance();
//    void send(IpcEvent event, const EventValue& value)
//    {
//        std::string eventStr = toStringMap.at(event);
//        Game::getInstance()->event()->emit(eventStr, value);
//    }
//    template <typename T, typename Func>
//    uint64_t on(IpcEvent event, Func func, T *instance)
//    {
//        std::string eventStr = toStringMap.at(event);
//        return Game::getInstance()->event()->on(eventStr, func, instance);
//    }
//};
