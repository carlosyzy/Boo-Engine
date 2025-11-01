#include "node-2d.h"
#include <iostream>


Node2D::Node2D(const std::string& name, const std::string& uuid) : NodeBase(name, uuid) {
}

Node2D::~Node2D() {
}
void Node2D::update() {
    // 调用基类的更新方法
    NodeBase::update();
    // 2D节点特有的更新逻辑
    std::cout << "Node2D update: " << _name << " at (" << _position.x() << ", " << _position.y() << ")" << std::endl;
}

void Node2D::render() {
    // 调用基类的渲染方法
    NodeBase::render();
    // 2D节点特有的渲染逻辑
    if (_active) {
        std::cout << "Node2D render: " << _name << " at (" << _position.x() << ", " << _position.y() << ")" << std::endl;
    }
}