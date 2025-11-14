// #pragma once
// #include <string>
// #include <iostream>
// #include <functional>
// #include <algorithm>
// #include <vector>
// class Node;
// class Node2D;
// class Scene;

// struct NodeInputResult
// {
//     Node2D *node;
//     float worldX = 0;
//     float worldY = 0;
//     /** 按钮 
//      * 0 左键
//      * 1 右键
//     */
//     int button = 0;
// };
// enum class NodeInput
// {
//     TOUCH_START,
//     TOUCH_MOVE,
//     TOUCH_END,
//     TOUCH_CANCEL
// };

// /**
//  * 节点的输入事件
//  * @brief 节点输入结构体
//  */
// struct NodeInputStruct
// {
//     // 事件绑定节点
//     Node2D *node = nullptr;
//     // 拦截事件
//     bool isIntercept = false;
//     // 状态 0 正常 1 选中
//     int status = 0;
//     // 返回触摸事件参数
//     NodeInputResult touchResult{};
//     // 事件回调函数
//     std::vector<std::function<void(NodeInputResult &)>> touchStarts;
//     std::vector<std::function<void(NodeInputResult &)>> touchMoves;
//     std::vector<std::function<void(NodeInputResult &)>> touchEnds;
//     std::vector<std::function<void(NodeInputResult &)>> touchCancels;
// };

// class InputMgr
// {
// private:
//     InputMgr() = default;
//     ~InputMgr() = default;
//     InputMgr(const InputMgr &) = delete;            // 禁用拷贝构造
//     InputMgr &operator=(const InputMgr &) = delete; // 禁用赋值操作符
//     float _cursorX = 0;
//     float _cursorY = 0;
//     bool _isTouching = false;
//     // 当前绑定场景
//     Node2D *_root = nullptr;
//     /**
//      * @brief 事件回调ID映射表
//      */
//     int _nodeInputCallId = 0;
//     std::unordered_map<std::string, NodeInputStruct> _nodeInputMap;
//     void _addInpotEvent(Node2D *node, std::function<void(NodeInputResult &)> callback, std::vector<std::function<void(NodeInputResult &)>> &eventList);
//     bool _propagateEvent(Node *node, int button, int action);
//     // // 节点输入事件映射表
//     // std::unordered_map<std::string, NodeInputStruct> _nodeInputMap;
//     // void _removeInputEvent(Node *node, std::function<void(NodeInputResult &)> callback, std::vector<std::function<void(NodeInputResult &)>> &eventList);

//     // // 获取最顶层节点
//     // Node *_getInputNodeOfTopFloor(Node *node);
//     // // 事件冒泡处理
//     //
//     // bool _isHitOnUI(Node *node, float x, float y);

// public:
//     static InputMgr *getInstance();
//     void init();
//     void setRoot(Node2D *root);
//     void onMouseButton(int button, int action, int mods);
//     void onCursorPos(double xpos, double ypos);
//     // // 节点输入事件
//     int onNodeInputEvent(Node2D *node, NodeInput input, std::function<void(NodeInputResult &)> callback, bool isIntercept = false);
//     // 取消节点输入事件
//     void offNodeInputEvent(Node2D *node, NodeInput input);
//     // 取消所有节点输入事件
//     void offAllNodeInputEvent(Node2D *node);
// };
