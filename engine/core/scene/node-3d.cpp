#include "node-3d.h"
#include <iostream>

Node3D::Node3D(const std::string name, const std::string uuid) : Node(name, uuid) {
    this->_layer = NodeLayer::Node3D;
}

Node3D::~Node3D() {
    
}

void Node3D::update(float dt) {
	Node::update(dt);
	std::cout << "Node update: " << _name << std::endl;
}
void Node3D::lateUpdate(float dt) {
	Node::lateUpdate(dt);
	std::cout << "Node lateUpdate: " << _name << std::endl;
}
void Node3D::render() {
	Node::render();
	if (_active) {
		std::cout << "Node render: " << _name << std::endl;
	}
}