#pragma once
#include <iostream>
#include <string>

class Node;
enum class NodeLayer;

class Component
{

private:
    bool _isAwaked = true;

protected:
    std::string _uuid;
    std::string _name;
    NodeLayer _layer;
    Node *_node;
    bool _isEnabled = true;
    bool _isEnabledInHierarchy = false;
    // 反序列化
    virtual void _deserialized() = 0;
public:
    Component(std::string name, Node *node, std::string uuid = "");
    const std::string getUuid() { return this->_uuid; }

    virtual void Awake() = 0;
    
    bool isEnabled() { return this->_isEnabled; }
    void setEnabled(bool enabled);
    std::string getName() { return this->_name; }
    Node *getNode() { return this->_node; }
    NodeLayer layer() { return this->_layer; }
    /**
     * @brief 设置节点是否激活在层级中
     * @param isActiveInHierarchy 是否激活在层级中
     */
    void setNodeActiveInHierarchy(bool isActiveInHierarchy);
    const bool isEnabledInHierarchy(){return this->_isEnabledInHierarchy;}
    virtual void Enable() = 0;
    /**
     * @brief 组件更新函数
     * @param deltaTime 时间步长
     * 当前节点组件以及子节点组件的update更新函数开始
     */
    virtual void Update(float deltaTime) = 0;
    /**
     * @brief 组件晚更新函数
     * @param deltaTime 时间步长
     * 当前节点组件以及子节点组件的update执行结束,lateUpdate更新函数开始
     */
    virtual void LateUpdate(float deltaTime) = 0;
    /**
     * @brief 组件渲染函数
     * 当前节点组件以及子节点组件的lateUpdate执行结束,render渲染函数开始
     */
    virtual void Render() = 0;
    /**
     * @brief 组件禁用函数
     * 当前节点组件以及子节点组件的disable禁用函数开始
     */
    virtual void Disable() = 0;
    /**
     * @brief 组件销毁函数
     */
    virtual void destroy();
    virtual ~Component();
};
