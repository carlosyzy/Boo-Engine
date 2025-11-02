#include "node-3d.h"
#include <iostream>

Node3D::Node3D(const std::string name, const std::string uuid) : Node(name, uuid) {
    this->_layer = NodeLayer::Node3D;
}




Component* Node3D::addComponent(std::string name, std::string uuid) {
	return nullptr;
}
/*
* 获取组件
*/
Component* Node3D::getComponent(std::string name) {
	return nullptr;
}



void Node3D::update(float dt) {
	Node::update(dt);
}
void Node3D::lateUpdate(float dt) {
	Node::lateUpdate(dt);
}
void Node3D::render() {
	Node::render();
}



void Node3D::clearNodeFrameFlag() {
	Node::clearNodeFrameFlag();
}
void Node3D::destroy() {
	Node::destroy();
}
Node3D::~Node3D() {

}