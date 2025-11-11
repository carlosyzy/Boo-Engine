#include "node-3d.h"
#include "../utils/uuid-util.h"
#include <iostream>

Node3D::Node3D(const std::string name, const std::string uuid)
{
	this->_name = name;
	this->_layer = NodeLayer::Node3D;
	this->_uuid = uuid.empty() ? UuidUtil::generateUUID() : uuid;
	this->_active = true;
	this->_isActiveInHierarchy = true;
	this->_position.set(0.0f, 0.0f, 0.0f);
	this->_scale.set(1.0f, 1.0f, 1.0f);
	this->_eulerAngles.set(0.0f, 0.0f, 0.0f);
	this->_rotation.set(0.0f, 0.0f, 0.0f, 1.0f);
	this->_worldPosition.set(0.0f, 0.0f, 0.0f);
	this->_worldScale.set(1.0f, 1.0f, 1.0f);
	this->_worldRotation.set(0.0f, 0.0f, 0.0f, 1.0f);
	this->_localMatrix = Mat4::identity();
	this->_worldMatrix = Mat4::identity();
	this->_worldTransformFlag = static_cast<uint32_t>(NodeTransformFlag::ALL_FLAG);
	this->_frameTransformFlag = static_cast<uint32_t>(NodeTransformFlag::ALL_FLAG);
}

Component *Node3D::addComponent(std::string name, std::string uuid)
{
	return nullptr;
}
/*
 * ��ȡ���
 */
Component *Node3D::getComponent(std::string name)
{
	return nullptr;
}

void Node3D::update(float dt)
{
	Node::update(dt);
}
void Node3D::lateUpdate(float dt)
{
	Node::lateUpdate(dt);
}
void Node3D::render()
{
	Node::render();
}

void Node3D::clearNodeFrameFlag()
{
	Node::clearNodeFrameFlag();
}
void Node3D::destroy()
{
	Node::destroy();
}
Node3D::~Node3D()
{
}