#include "node-3d.h"
#include <iostream>

Node3D::Node3D(const std::string name, const std::string uuid) : Node(name, uuid) {
    
}

Node3D::~Node3D() {
    
}

void Node3D::update(float dt) {
	Node::update(dt);
	// 基础更新逻辑，子类可以重写
	std::cout << "Node update: " << _name << std::endl;
}
void Node3D::lateUpdate(float dt) {
	Node::lateUpdate(dt);
	// 基础更新逻辑，子类可以重写
	std::cout << "Node lateUpdate: " << _name << std::endl;
}
void Node3D::render() {
	Node::render();
	// 基础渲染逻辑，子类可以重写
	if (_active) {
		std::cout << "Node render: " << _name << std::endl;
	}
}