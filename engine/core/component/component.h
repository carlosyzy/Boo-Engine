#pragma once
#include <iostream>
#include <string>

class Node;
enum class NodeLayer;

class Component
{

protected:
    std::string _uuid;
    std::string _name;
    NodeLayer _layer;
    Node *_node;
    bool _isEnabled = true;
    int _priority;
public:
    Component(Node *node, std::string uuid = "");
    bool isEnabled() { return this->_isEnabled; }
    void setEnabled(bool enabled);
    Node *getNode() { return this->_node; }
    NodeLayer layer() { return this->_layer; }
    /**
     * @brief 组件更新函数
     * @param deltaTime 时间步长
     * 当前节点组件以及子节点组件的update更新函数开始
     */
    virtual void update(float deltaTime) = 0;
    /**
     * @brief 组件晚更新函数
     * @param deltaTime 时间步长
     * 当前节点组件以及子节点组件的update执行结束,lateUpdate更新函数开始
     */
    virtual void lateUpdate(float deltaTime) = 0;
    /**
     * @brief 组件渲染函数
     * 当前节点组件以及子节点组件的lateUpdate执行结束,render渲染函数开始
     */
    virtual void render() = 0;
    /**
     * @brief 组件晚渲染函数 
     * 当前节点组件以及子节点组件的render执行结束,lateRender渲染函数开始 
     */
    virtual void lateRender() = 0;
    /**
     * @brief 组件销毁函数
     */
    virtual void destroy();
    virtual ~Component();
};
