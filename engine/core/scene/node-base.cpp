#include "node-base.h"
#include <iostream>
#include <random>
#include <sstream>
#include <iomanip>
#include "../utils/uuid-util.h"


NodeBase::NodeBase(const std::string& name, const std::string& uuid)
    : _name(name), _active(true), _layer(NodeType::Node),
    _position(0.0f, 0.0f, 0.0f),
    _scale(1.0f, 1.0f, 1.0f),
    _eulerAngles(0.0f, 0.0f, 0.0f) {
    _uuid = uuid.empty() ? UuidUtil::generateUUID() : uuid;
}

NodeBase::~NodeBase() {
}

void NodeBase::setName(const std::string& name) {
    _name = name;
}

std::string NodeBase::getName() const {
    return _name;
}

void NodeBase::setActive(bool active) {
    _active = active;
}

bool NodeBase::isActive() const {
    return _active;
}

NodeType NodeBase::getLayer() const {
    return _layer;
}

std::string NodeBase::getUuid() const {
    return _uuid;
}

void NodeBase::setPosition(const Vec3& position) {
    _position = position;
}

const Vec3& NodeBase::getPosition() const {
    return _position;
}

void NodeBase::setScale(const Vec3& scale) {
    _scale = scale;
}

const Vec3& NodeBase::getScale() const {
    return _scale;
}

void NodeBase::setEulerAngles(const Vec3& eulerAngles) {
    _eulerAngles = eulerAngles;
}

const Vec3& NodeBase::getEulerAngles() const {
    return _eulerAngles;
}

void NodeBase::update() {
    // 基础更新逻辑，子类可以重写
    std::cout << "NodeBase update: " << _name << std::endl;
}

void NodeBase::render() {
    // 基础渲染逻辑，子类可以重写
    if (_active) {
        std::cout << "NodeBase render: " << _name << std::endl;
    }
}