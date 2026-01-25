#include "node.h"
#include "../utils/uuid-util.h"
#include "../component/component.h"
#include "../component/ui/ui-widget.h"
#include "../../boo.h"
#include "../renderer/ui/ui-renderer.h"

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
	this->_active = active;
	if (this->_parent == nullptr)
	{
		// 不存在父节点，只存储状态
		return;
	}
	// 子节点是否激活 = 父节点是否激活 && 子节点是否激活
	bool _isActiveInHierarchy = this->_parent->_isActiveInHierarchy && this->_active;
	this->_updateNodesActiveInHierarchyState(_isActiveInHierarchy);
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
void Node::setUuid(const std::string &uuid)
{
	this->_uuid = uuid;
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
void Node::addChild(Node *child)
{
	child->removeFromParent();
	child->setParent(this);
}
void Node::setParent(Node *parent)
{
	if (nullptr == parent)
	{
		return;
	}
	// 检测是否已经在父节点中
	for (auto &child : parent->_children)
	{
		if (child == this)
		{
			std::cout << "Node::setParent: " << this->_name << " already in parent " << this->_parent->_name << std::endl;
			return;
		}
	}
	this->_parent = parent;
	this->_parent->_children.push_back(this);
	this->_visibility = parent->_visibility;
	// 更新节点结构激活状态
	this->setActive(this->_active);
}

void Node::_updateWorldTransformFlag(NodeTransformFlag flag)
{
	this->_worldTransformFlag |= static_cast<uint32_t>(flag);
	this->_frameTransformFlag |= static_cast<uint32_t>(flag);
	this->_emitTransformChanged(this->_worldTransformFlag);
	// 更新渲染节点的模型矩阵
	for (auto &child : this->_children)
	{
		child->_updateWorldTransformFlag(flag);
	}
}
void Node::_updateNodesActiveInHierarchyState(bool isActiveInHierarch)
{
	if (this->_isActiveInHierarchy == isActiveInHierarch)
		return;
	this->_isActiveInHierarchy = isActiveInHierarch;
	// std::cout << "Node::_updateNodesActiveInHierarchyState: " << this->_name << " isActiveInHierarch: " << isActiveInHierarch << std::endl;
	// 更新组件生命周期
	for (auto &component : this->_components)
	{
		component->setNodeActiveInHierarchy(isActiveInHierarch);
	}

	for (auto &child : this->_children)
	{
		child->_updateNodesActiveInHierarchyState(isActiveInHierarch);
	}
}
void Node::_updateWorldTransform()
{
	if (!this->_isActiveInHierarchy)
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
	this->_worldScale.set(this->_worldMatrix.getM00(), this->_worldMatrix.getM11(), this->_worldMatrix.getM22());
	this->_worldTransformFlag = NodeTransformFlag::NONE_FLAG;
}

/**
 * 添加组件
 */
Component *Node::addComponent(std::string name, std::string uuid)
{
	return nullptr;
}
/*
 * 获取组件
 */
Component *Node::getComponent(std::string name)
{
	for (auto component : this->_components)
	{
		if (component != nullptr)
		{
			return component;
		}
	}
	return nullptr;
}
/**
 * 获取所有组件
 */
std::vector<Component *> Node::getComponents()
{
	return this->_components;
}

void Node::update(float dt)
{
	// 更新组件
	for (auto &component : this->_components)
	{
		if (!component->isEnabledInHierarchy())
			continue;
		component->Update(dt);
	}
	// 更新子节点
	for (auto &child : this->_children)
	{
		child->update(dt);
	}
}
void Node::lateUpdate(float dt)
{
	// ui-widget 组件 比较特殊，特出处理
	for (auto &component : this->_components)
	{
		if (!component->isEnabledInHierarchy())
		{
			continue;
		}
		if (this->_layer == NodeLayer::Node2D)
		{
			UIWidget *uiWidget = dynamic_cast<UIWidget *>(component);
			if (uiWidget != nullptr)
			{
				uiWidget->updateWidget();
				continue;
			}
			// UIMask *uiMask = dynamic_cast<UIMask *>(component);
			// if (uiMask != nullptr)
			// {
			// 	uiMask->updateMask();
			// 	continue;
			// }
		}
	}
	// 更新组件
	for (auto &component : this->_components)
	{
		if (!component->isEnabledInHierarchy())
			continue;
		component->LateUpdate(dt);
	}
	for (auto &child : this->_children)
	{
		child->lateUpdate(dt);
	}
}
// void Node::render()
// {
// 	// 渲染组件
// 	for (auto &component : this->_components)
// 	{
// 		if (!component->isEnabledInHierarchy())
// 			continue;
// 		component->Render();
// 	}
// 	for (auto &child : this->_children)
// 	{
// 		child->render();
// 	}
// }
void Node::clearNodeFrameFlag()
{
	this->_frameTransformFlag = NodeTransformFlag::NONE_FLAG;
	for (auto &child : this->_children)
	{
		child->clearNodeFrameFlag();
	}
}

/**
 * 从父节点中移除子节点
 */
void Node::removeFromParent()
{
	if (this->_parent!=nullptr)
	{
		this->_parent->removeChild(this);
	}
	this->_parent = nullptr;
}
void Node::removeChild(Node *node)
{
	node->_updateNodesActiveInHierarchyState(false);
	// 使用标准算法更安全
	auto it = std::find(this->_children.begin(), this->_children.end(), node);
	if (it != this->_children.end())
	{
		this->_children.erase(it);
	}
}
void Node::clearAllEvent()
{
	this->_transformListeners.clear();
}

void Node::destroyAllChildren()
{
	auto childrenCopy = this->_children;
	this->_children.clear(); // 先清空，防止递归调用时的修改
	for (auto &child : childrenCopy)
	{
		child->destroy(); // 安全销毁
	}
}
void Node::destroyAllComponents()
{
	for (auto &component : this->_components)
	{
		component->destroy();
	}
	this->_components.clear();
}

void Node::destroy()
{
	// 从父节点中移除-此时所有组件，包括子节点都置为未激活状态
	this->removeFromParent();
	// 清楚当前节点所有事件
	this->clearAllEvent();
	// 销毁当前节点所有组件
	this->destroyAllComponents();
	// 递归销毁所有子节点
	this->destroyAllChildren();

	Boo::game->addNodeClearCaches(this);
}