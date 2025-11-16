#include "node-2d.h"
#include "../../boo.h"
#include "../input/input.h"
#include "../component/component.h"
#include "../renderer/ui/ui-renderer.h"

Node2D::Node2D(const std::string name, const std::string uuid)
{
	this->_name = name;
	this->_layer = NodeLayer::Node2D;
	this->_uuid = uuid.empty() ? UuidUtil::generateUUID() : uuid;
	this->_anchor.set(0.5, 0.5);
	this->_size.set(200, 200);
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
	std::cout << "Node2D::setSize: " << width << ", " << height << std::endl;
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
	this->_uiWorldMatrix.setM00(this->_uiWorldMatrix.getM00() * this->_size.getWidth()); // 宽高和缩放进行相乘
	this->_uiWorldMatrix.setM11(this->_uiWorldMatrix.getM11() * this->_size.getHeight());
	// 锚点
	this->_uiWorldMatrix.setM30(this->_uiWorldMatrix.getM30() + (0.5 - this->_anchor.getX()) * this->_size.getWidth());
	this->_uiWorldMatrix.setM31(this->_uiWorldMatrix.getM31() + (0.5 - this->_anchor.getY()) * this->_size.getHeight());
}

Component *Node2D::addComponent(std::string name, std::string uuid)
{
	Component *component = Boo::game->createComponent(name, this, uuid);
	if (component == nullptr)
	{
		std::cout << name << ":Component Not  register" << std::endl;
		return nullptr;
	}
	if (component->layer() != NodeLayer::Node2D)
	{
		std::cout << name << ":Component add fail,node type not is Node2D" << std::endl;
		delete component;
		component = nullptr;
		return nullptr;
	}
	this->_components.push_back(component);
	return component;
}
/*
 * 获取组件
 */
Component *Node2D::getComponent(std::string name)
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

void Node2D::update(float dt)
{
	Node::update(dt);
}
void Node2D::lateUpdate(float dt)
{
	Node::lateUpdate(dt);
}
void Node2D::render()
{
	Node::render();
}
/**
 * 刷新渲染组件的模型矩阵
 */
void Node2D::_updateRendererModelMatrix()
{
	Node::_updateRendererModelMatrix();
	for (auto &component : this->_components)
	{
		UIRenderer *uiRenderer = dynamic_cast<UIRenderer *>(component);
		if (uiRenderer != nullptr)
		{
			uiRenderer->updateModelMatrix();
		}
	}
}

void Node2D::clearNodeFrameFlag()
{
	Node::clearNodeFrameFlag();
}
// template <typename T, typename Func>
// int Node2D::onNodeInputEvent(NodeInput input, Func func, T *instance, bool isIntercept)
// {
// 	return Boo::game->input()->onNodeInputEvent(this, input, func, instance, isIntercept);
// }
void Node2D::offNodeInputEvent(int inputID)
{
	Boo::game->input()->offNodeInputEvent(this, inputID);
}
void Node2D::offAllNodeInputEvent()
{
	Boo::game->input()->offAllNodeInputEvent(this);
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
	const Mat4 &uiMat = this->uiWorldMatrix();
	float _x = uiMat.getM30();
	float _y = uiMat.getM31();
	float _width = uiMat.getM00();
	float _height = uiMat.getM11();
	if (
		x >= _x - _width / 2.0 && x <= _x + _width / 2.0 &&
		y >= _y - _height / 2.0 && y <= _y + _height / 2.0)
	{
		return true;
	}

	return false;
}

void Node2D::destroy()
{
	this->offAllNodeInputEvent();
	Node::destroy();
}
Node2D::~Node2D()
{
	std::cout << "Node2D::~destructor: " << this->_name << std::endl;
}