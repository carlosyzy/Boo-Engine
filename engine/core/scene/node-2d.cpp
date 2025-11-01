#include "node-2d.h"
#include <iostream>

Node2D::Node2D(const std::string name, const std::string uuid) : Node(name, uuid) {
    
}

Node2D::~Node2D() {
    
}

void Node2D::update(float dt) {
	Node::update(dt);
	// 基础更新逻辑，子类可以重写
	std::cout << "Node update: " << _name << std::endl;
}
void Node2D::lateUpdate(float dt) {
	Node::lateUpdate(dt);
	// 基础更新逻辑，子类可以重写
	std::cout << "Node lateUpdate: " << _name << std::endl;
}
void Node2D::render() {
	Node::render();
	// 基础渲染逻辑，子类可以重写
	if (_active) {
		std::cout << "Node render: " << _name << std::endl;
	}
}