#include "node-2d.h"
#include "../../boo.h"
#include "../input/input.h"
#include "../component/component.h"
#include "../component/component-factory.h"
#include "../renderer/ui/ui-renderer.h"
#include "../renderer/ui/ui-mask.h"

Node2D::Node2D(const std::string name, const std::string uuid)
{
	this->_name = name;
	this->_layer = NodeLayer::Node2D;
	this->_uuid = uuid.empty() ? UuidUtil::generateUUID() : uuid;
	this->_anchor.set(0.5, 0.5);
	this->_size.set(200, 200);
	this->_active = true;
	this->_isActiveInHierarchy = false;
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
	this->_visibility = uint32_t(NodeVisibility::Node2D);
}

/**
 * 2d 节点的锚点
 * @param x
 * @param y
 */
void Node2D::setAnchor(float x, float y)
{
	if (this->_anchor.getX() == x && this->_anchor.getY() == y)
	{
		return;
	}
	this->_anchor.set(x, y);
	this->_updateWorldTransformFlag(NodeTransformFlag::ANCHOR_FLAG);
}
/**
 * 2d 节点的大小
 * @param width
 * @param height
 */
void Node2D::setSize(float width, float height)
{
	if (this->_size.getWidth() == width && this->_size.getHeight() == height)
	{
		return;
	}
	this->_size.set(width, height);
	this->_updateWorldTransformFlag(NodeTransformFlag::SIZE_FLAG);
}

/**
 * 2d 节点的世界变换矩阵
 */
void Node2D::_updateWorldTransform()
{
	Node::_updateWorldTransform();
	// 尺寸
	this->_uiWorldMatrix.set(this->_worldMatrix);
	float _x = this->_uiWorldMatrix.getM30() + (0.5 - this->_anchor.getX()) * this->_size.getWidth();
	float _y = this->_uiWorldMatrix.getM31() + (0.5 - this->_anchor.getY()) * this->_size.getHeight();
	float _width = this->_uiWorldMatrix.getM00() * this->_size.getWidth();
	float _height = this->_uiWorldMatrix.getM11() * this->_size.getHeight();
	// 锚点
	this->_uiWorldMatrix.setM30(_x);
	this->_uiWorldMatrix.setM31(_y);
	this->_uiWorldMatrix.setM00(_width); // 宽高和缩放进行相乘
	this->_uiWorldMatrix.setM11(_height);
}
Component *Node2D::addComponent(std::string name, std::string uuid)
{
	Component *component = ComponentFactory::getInstance().createComponent(name, this, uuid);
	if (component == nullptr)
	{
		std::cout << name << ":Component Not register" << std::endl;
		return nullptr;
	}
	if (component->layer() == NodeLayer::Node3D)
	{
		std::cout << name << ":Component add fail,node type is Node3D" << std::endl;
		delete component;
		component = nullptr;
		return nullptr;
	}
	this->_components.push_back(component);
	if (this->_parent != nullptr)
	{
		component->setNodeActiveInHierarchy(this->_isActiveInHierarchy);
	}
	return component;
}
/**
 * 获取UI渲染组件
 */
UIRenderer *Node2D::getUIRenderComponent()
{
	for (auto &component : this->_components)
	{
		UIRenderer *uiRenderer = dynamic_cast<UIRenderer *>(component);
		if (uiRenderer != nullptr)
		{
			return uiRenderer;
		}
	}
	return nullptr;
}

void Node2D::update(float dt)
{
	Node::update(dt);
}
void Node2D::lateUpdate(float dt)
{
	Node::lateUpdate(dt);
}
// void Node2D::render()
// {
// 	Node::render();
// 	for (auto &component : this->_components)
// 	{
// 		if (!component->isEnabledInHierarchy())
// 			continue;
// 		UIMask *uiMask = dynamic_cast<UIMask *>(component);
// 		if (uiMask != nullptr)
// 		{
// 			uiMask->lateRender();
// 		}
// 	}
// }
void Node2D::clearNodeFrameFlag()
{
	Node::clearNodeFrameFlag();
}
void Node2D::offNodeInputEvent(int inputID)
{
	Boo::game->input()->offNodeInputEvent(this, inputID);
}
void Node2D::offAllNodeInputEvent()
{
	Boo::game->input()->offAllNodeInputEvent(this);
}
bool Node2D::inHitMask(float x, float y)
{
	const Mat4 &uiMat = this->uiWorldMatrix();
	float _x = uiMat.getM30();
	float _y = uiMat.getM31();
	float _width = uiMat.getM00();
	float _height = uiMat.getM11();
	if (x >= _x - _width / 2.0 && x <= _x + _width / 2.0 && y >= _y - _height / 2.0 && y <= _y + _height / 2.0)
	{
		return true;
	}
	return false;
}
/**
 * 2d 节点的点击事件
 * @param x
 * @param y
 * @return true
 * @return false
 */
bool Node2D::inHitOnNode(float x, float y)
{
	// UI Mask属于特殊情况，必须在ui-node里边，并且同时在ui-mask里边
	const Mat4 &uiMat = this->uiWorldMatrix();
	float _x = uiMat.getM30();
	float _y = uiMat.getM31();
	float _width = uiMat.getM00();
	float _height = uiMat.getM11();

	float _left = _x - _width / 2.0;
	float _right = _x + _width / 2.0;
	float _top = _y - _height / 2.0;
	float _bottom = _y + _height / 2.0;
	// std::cout << "Node2D::inHitOnNode: " << this->_name << " left: " << _left << " right: " << _right << " top: " << _top << " bottom: " << _bottom  << "width: " << _width << " height: " << _height << std::endl;
	if (x >= _left && x <= _right && y >= _top && y <= _bottom)
	{
		// std::cout << "Node2D::inHitOnNode: " << this->_name << " is hit" << std::endl;
		return true;
	}
	return false;
}

void Node2D::clearAllEvent()
{
	Node::clearAllEvent();
	this->offAllNodeInputEvent();
}

void Node2D::destroy()
{
	Node::destroy();
}
Node2D::~Node2D()
{
	std::cout << "Node2D::~destructor: " << this->_name << std::endl;
}