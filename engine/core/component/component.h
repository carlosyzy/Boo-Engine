#pragma once
#include <iostream>
#include <string>

class Node;
enum class NodeLayer;

// /**
//  * @brief 组件优先级
//  * 值越大，优先级越高
//  */
// enum ComponentPriority
// {
//     Renderer = -1, // 后续这个废除
//     Default = 0,
//     Mask = 100, //
//     Widget = 200, //
// };

// enum ComponentFlag
// {
//     None = 0,
//     Loaded = 1 << 0,
//     Enabled = 1 << 1,
// };

class Component
{

protected:
    std::string _uuid;
    NodeLayer _layer;
    Node *_node;
    bool _isEnabled = true;
    int _priority;

public:
    Component(Node *node, std::string uuid = "");
    ~Component();
    bool isEnabled() { return this->_isEnabled; }
    void setEnabled(bool enabled);
    virtual void update(float deltaTime) = 0;
    NodeLayer layer() { return this->_layer; }
    void destroy();
};
