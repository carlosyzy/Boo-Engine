#include "node-3d.h"
#include <iostream>

Node3D::Node3D(const std::string& name, const std::string& uuid) : NodeBase(name, uuid) {
}

Node3D::~Node3D() {
}

void Node3D::update() {
    // 调用基类的更新方法
    NodeBase::update();
    // 3D节点特有的更新逻辑
    std::cout << "Node3D update: " << _name << " at (" << _position.x() << ", " << _position.y() << ", " << _position.z() << ")" << std::endl;
}

void Node3D::render() {
    // 调用基类的渲染方法
    NodeBase::render();
    // 3D节点特有的渲染逻辑
    if (_active) {
        std::cout << "Node3D render: " << _name << " at (" << _position.x() << ", " << _position.y() << ", " << _position.z() << ")" << std::endl;
    }
}