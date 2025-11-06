#include "node.h"
#include "../utils/uuid-util.h"
#include "../component/component.h"

Node::Node(const std::string name, const std::string uuid)
	: _name(name), _active(true), _layer(NodeLayer::Node), _isActiveInHierarchy(true),
	  _position(0.0f, 0.0f, 0.0f), _scale(1.0f, 1.0f, 1.0f),
	  _eulerAngles(0.0f, 0.0f, 0.0f), _rotation(0.0f, 0.0f, 0.0f, 1.0f),
	  _worldPosition(0.0f, 0.0f, 0.0f), _worldScale(1.0f, 1.0f, 1.0f),
	  _worldRotation(0.0f, 0.0f, 0.0f, 1.0f),
	  _localMatrix(Mat4::identity()), _worldMatrix(Mat4::identity()),
	  _worldTransformFlag(NodeTransformFlag::ALL_FLAG)
{
	_uuid = uuid.empty() ? UuidUtil::generateUUID() : uuid;
}

void Node::setName(const std::string &name)
{
	this->_name = name;
}

std::string Node::getName() const
{
	return this->_name;
}

void Node::setActive(bool active, bool force)
{
	if (this->_active == active && !force)
	{
		return;
	}
	this->_active = active;
	this->_isActiveInHierarchy = ((this->_parent == nullptr) ? true : this->_parent->_isActiveInHierarchy) && this->_active;
	this->_updateNodesActiveInHierarchyState(this->_isActiveInHierarchy);
}

bool Node::isActive() const
{
	return this->_active;
}

const NodeLayer Node::getLayer() const
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
	this->_localMatrix.translate(x, y, z);
	this->_updateWorldTransformFlag(NodeTransformFlag::POSITION_FLAG);
	this->emit(NodeEvent::ON_TRANSFORM_CHANGED);
}
void Node::setWorldPosition(float x, float y, float z)
{
	// if (this->_worldPosition.getX() == x && this->_worldPosition.getY() == y && this->_worldPosition.getZ() == z)
	// {
	// 	return;
	// }
	// this->_worldPosition.set(x, y, z);
	// this->_updateWorldTransformFlag(NodeTransformFlag::POSITION_FLAG);
}
/**
 * 设置四元素角度
 */
void Node::setRotation(float x, float y, float z, float w)
{
	// if (this->_rotation.getX() == x && this->_rotation.getY() == y && this->_rotation.getZ() == z && this->_rotation.getW() == w)
	// {
	// 	return;
	// }
	// this->_rotation.set(x, y, z, w);
	// this->_localMatrix.rotate(x, y, z, w);
	// this->_updateWorldTransformFlag(NodeTransformFlag::ROTATION_FLAG);
}
void Node::setWorldRotation(float x, float y, float z, float w)
{
	// if (this->_worldRotation.getX() == x && this->_worldRotation.getY() == y && this->_worldRotation.getZ() == z && this->_worldRotation.getW() == w)
	// {
	// 	return;
	// }
	// this->_worldRotation.set(x, y, z, w);
	// this->_updateWorldTransformFlag(NodeTransformFlag::ROTATION_FLAG);
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
	this->_localMatrix.scale(x, y, z);
	this->_updateWorldTransformFlag(NodeTransformFlag::SCALE_FLAG);
	this->emit(NodeEvent::ON_TRANSFORM_CHANGED);
}
void Node::setWorldScale(float x, float y, float z)
{
	// if (this->_worldScale.getX() == x && this->_worldScale.getY() == y && this->_worldScale.getZ() == z)
	// {
	// 	return;
	// }
	// this->_worldScale.set(x, y, z);
	// this->_localMatrix.scale(x, y, z);
	// this->_updateWorldTransformFlag(NodeTransformFlag::SCALE_FLAG);
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
	this->setActive(this->_active, true);
}

void Node::removeChild(Node *node)
{
	for (auto it = this->_children.begin(); it != this->_children.end();)
	{
		if (*it == node)
		{
			it = this->_children.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Node::_updateWorldTransformFlag(NodeTransformFlag flag)
{
	this->_worldTransformFlag |= static_cast<uint32_t>(flag);
	this->_frameTransformFlag |= static_cast<uint32_t>(flag);
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
		Mat4::multiply(this->_localMatrix, this->_parent->getWorldMatrix(), this->_worldMatrix);
	}
	else
	{
		this->_worldMatrix = this->_localMatrix;
	}
	this->_worldPosition.set(this->_worldMatrix.getM30(), this->_worldMatrix.getM31(), this->_worldMatrix.getM32());
	this->_worldTransformFlag = NodeTransformFlag::NONE_FLAG;
}

void Node::update(float dt)
{
	if (!this->_isActiveInHierarchy)
	{
		return;
	}

	// 更新组件
	for (auto &component : this->_components)
	{
		component->update(dt);
	}
	// 更新子节点
	for (auto &child : this->_children)
	{
		child->update(dt);
	}
}
void Node::lateUpdate(float dt)
{
	if (!this->_isActiveInHierarchy)
	{
		return;
	}
	// 更新组件
	for (auto &component : this->_components)
	{
		component->lateUpdate(dt);
	}
	for (auto &child : this->_children)
	{
		child->lateUpdate(dt);
	}
}
void Node::render()
{
	if (!this->_isActiveInHierarchy)
	{
		return;
	}

	// 渲染组件
	for (auto &component : this->_components)
	{
		component->render();
	}
	for (auto &child : this->_children)
	{
		child->render();
	}
}
void Node::clearNodeFrameFlag()
{
	this->_frameTransformFlag = NodeTransformFlag::NONE_FLAG;
	for (auto &child : this->_children)
	{
		child->clearNodeFrameFlag();
	}
}

Component *Node::addComponent(std::string name, std::string uuid)
{
	return nullptr;
}
/*
 * 获取组件
 */
Component *Node::getComponent(std::string name)
{
	return nullptr;
}

void Node::destroyAllChildren()
{
	// 递归销毁所有子节点
	for (auto &child : this->_children)
	{
		child->destroy();
	}
	this->_children.clear();
}
void Node::destroy()
{
	// 销毁组件
	for (auto &component : this->_components)
	{
		component->destroy();
	}
	this->_components.clear();
	// 递归销毁所有子节点
	this->destroyAllChildren();
	delete this;
}
Node ::~Node()
{
}
