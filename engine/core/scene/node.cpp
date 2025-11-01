#include "node.h"
#include "../utils/uuid-util.h"
#include <iostream>

Node::Node(const std::string name, const std::string uuid)
	: _name(name), _active(true), _layer(NodeType::Node), _isActiveInHierarchy(true),
	  _position(0.0f, 0.0f, 0.0f), _scale(1.0f, 1.0f, 1.0f),
	  _eulerAngles(0.0f, 0.0f, 0.0f), _rotation(0.0f, 0.0f, 0.0f, 1.0f),
	  _worldPosition(0.0f, 0.0f, 0.0f), _woildScale(1.0f, 1.0f, 1.0f),
	  _worldRotation(0.0f, 0.0f, 0.0f, 1.0f),
	  _worldTransformFlag(NodeTransformFlag::ALL_FLAG)
{
	_uuid = uuid.empty() ? UuidUtil::generateUUID() : uuid;
}

Node::~Node() {}

void Node::setName(const std::string &name)
{
	this->_name = name;
}

std::string Node::getName() const
{
	return this->_name;
}

void Node::setActive(bool active)
{
	if (this->_active == active)
	{
		return;
	}
	this->_active = active;
	this->_isActiveInHierarchy = (this->_parent == nullptr) ? true : this->_parent->_isActiveInHierarchy;
	bool isActive = this->_isActiveInHierarchy && this->_active;
	this->_updateNodesActiveInHierarchyState(isActive);
}

bool Node::isActive() const
{
	return this->_active;
}

const NodeType Node::getLayer() const
{
	return this->_layer;
}

const std::string Node::getUuid() const
{
	return this->_uuid;
}
/**
 * 设置坐标
 */
void Node::setPosition(float x, float y, float z)
{
	if (this->_position.getX() == x && this->_position.getY() == y && this->_position.getZ() == z)
	{
		return;
	}
	this->_position.set(x, y, z);
	this->_updateWorldTransformFlag(NodeTransformFlag::POSITION_FLAG);
}
void Node::setWorldPosition(float x, float y, float z)
{
	if (this->_worldPosition.getX() == x && this->_worldPosition.getY() == y && this->_worldPosition.getZ() == z)
	{
		return;
	}
	this->_worldPosition.set(x, y, z);
	this->_updateWorldTransformFlag(NodeTransformFlag::POSITION_FLAG);
}
/**
 * 设置四元素角度
 */
void Node::setRotation(float x, float y, float z, float w)
{
	if (this->_rotation.getX() == x && this->_rotation.getY() == y && this->_rotation.getZ() == z && this->_rotation.getW() == w)
	{
		return;
	}
	this->_rotation.set(x, y, z, w);
	this->_updateWorldTransformFlag(NodeTransformFlag::ROTATION_FLAG);
}
void Node::setWorldRotation(float x, float y, float z, float w)
{
	if (this->_worldRotation.getX() == x && this->_worldRotation.getY() == y && this->_worldRotation.getZ() == z && this->_worldRotation.getW() == w)
	{
		return;
	}
	this->_worldRotation.set(x, y, z, w);
	this->_updateWorldTransformFlag(NodeTransformFlag::ROTATION_FLAG);
}
/*
 * 设置缩放
 */
void Node::setScale(float x, float y, float z)
{
	if (this->_scale.getX() == x && this->_scale.getY() == y && this->_scale.getZ() == z)
	{
		return;
	}
	this->_scale.set(x, y, z);
	this->_updateWorldTransformFlag(NodeTransformFlag::SCALE_FLAG);
}
void Node::setWorldScale(float x, float y, float z)
{
	if (this->_woildScale.getX() == x && this->_woildScale.getY() == y && this->_woildScale.getZ() == z)
	{
		return;
	}
	this->_woildScale.set(x, y, z);
	this->_updateWorldTransformFlag(NodeTransformFlag::SCALE_FLAG);
}
void Node::setEulerAngles(float x, float y, float z)
{
	this->_eulerAngles.set(x, y, z);
	this->_updateWorldTransformFlag(NodeTransformFlag::ROTATION_FLAG);
}
void Node::addChild(Node *node)
{
	this->_children.push_back(node);
	node->setParent(this);
	// this->_frameChildFlag++;
}
void Node::setParent(Node *node)
{
	this->_parent = node;
	// 更新节点结构激活状态
	this->_isActiveInHierarchy = (this->_parent == nullptr) ? true : this->_parent->_isActiveInHierarchy;
	bool isActive = this->_isActiveInHierarchy && this->_active;
	this->_updateNodesActiveInHierarchyState(isActive);
}

void Node::removeChild(Node *node)
{
	this->_children.erase(std::remove(this->_children.begin(), this->_children.end(), node), this->_children.end());
    this->_frameChildFlag++;
}
void Node::destroyAllChildren()
{
}
void Node::_updateWorldTransformFlag(NodeTransformFlag flag)
{
	this->_worldTransformFlag |= static_cast<uint32_t>(flag);
	for (auto &child : this->_children)
	{
		child->_updateWorldTransformFlag(flag);
	}
}
void Node::_updateNodesActiveInHierarchyState(bool isActiveInHierarch)
{

	for (auto &child : this->_children)
	{
		child->_updateNodesActiveInHierarchyState(isActiveInHierarch);
	}
}
void Node::_updateWorldTransform()
{
	if (!this->_active)
		return;
	if (this->_worldTransformFlag == NodeTransformFlag::NONE_FLAG)
		return;
	if (this->_parent)
	{
		/*this->_worldMatrix = this->_parent->worldMatrix() * this->_localMatrix;*/
	}
	else
	{
		/*this->_worldMatrix = this->_localMatrix;*/
	}
	/*this->_worldPosition.set(this->_worldMatrix.m12(), this->_worldMatrix.m13(), this->_worldMatrix.m14());
	this->_worldTransformFlag = NodeTransformFlag::NONE_FLAG;*/
}

void Node::update(float dt)
{
	// 基础更新逻辑，子类可以重写
	std::cout << "Node update: " << _name << std::endl;
}
void Node::lateUpdate(float dt)
{
	// 基础更新逻辑，子类可以重写
	std::cout << "Node lateUpdate: " << _name << std::endl;
}
void Node::render()
{
	// 基础渲染逻辑，子类可以重写
	if (_active)
	{
		std::cout << "Node render: " << _name << std::endl;
	}
}