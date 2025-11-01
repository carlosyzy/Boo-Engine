#include "node.h"
#include "../utils/uuid-util.h"
#include <iostream>

Node::Node(const std::string name, const std::string uuid)
	: _name(name), _active(true), _layer(NodeType::Node),
	_position(0.0f, 0.0f, 0.0f), _scale(1.0f, 1.0f, 1.0f),
	_eulerAngles(0.0f, 0.0f, 0.0f), _rotate(0.0f, 0.0f, 0.0f, 1.0f),
	_worldPosition(0.0f, 0.0f, 0.0f), _woildScale(1.0f, 1.0f, 1.0f),
	_worldRotate(0.0f, 0.0f, 0.0f, 1.0f) {
	_uuid = uuid.empty() ? UuidUtil::generateUUID() : uuid;
}

Node::~Node() {}

void Node::setName(const std::string& name) {
	this->_name = name;
}

std::string Node::getName() const {
	return this->_name;
}

void Node::setActive(bool active) {
	this->_active = active;
}

bool Node::isActive() const {
	return this->_active;
}

NodeType Node::getLayer() const {
	return this->_layer;
}

std::string Node::getUuid() const {
	return this->_uuid;
}

void Node::setPosition(const Vec3& position) {
	this->_position = position;
}

const Vec3& Node::getPosition() const {
	return this->_position;
}

void Node::setScale(const Vec3& scale) {
	this->_scale = scale;
}

const Vec3& Node::getScale() const {
	return this->_scale;
}

void Node::setEulerAngles(const Vec3& eulerAngles) {
	this->_eulerAngles = eulerAngles;
}

const Vec3& Node::getEulerAngles() const {
	return this->_eulerAngles;
}
void Node::addChild(Node* node) {
	this->_children.push_back(node);
	node->setParent(this);
}
void Node::setParent(Node* node) {
	this->_parent = node;
}
void Node::removeChild(Node* node) {

}
void Node::destroyAllChildren() {

}

void Node::update(float dt) {
	// 基础更新逻辑，子类可以重写
	std::cout << "Node update: " << _name << std::endl;
}
void Node::lateUpdate(float dt) {
	// 基础更新逻辑，子类可以重写
	std::cout << "Node lateUpdate: " << _name << std::endl;
}
void Node::render() {
	// 基础渲染逻辑，子类可以重写
	if (_active) {
		std::cout << "Node render: " << _name << std::endl;
	}
}