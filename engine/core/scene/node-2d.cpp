#include "node-2d.h"
#include <iostream>
#include "node-2d.h"
Node2D::Node2D(const std::string name, const std::string uuid) : Node(name, uuid), _anchor(0.5, 0.5), _size(200, 200)
{
	this->_layer = NodeLayer::Node2D;
}

Node2D::~Node2D()
{
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
	this->emit(NodeEvent::ON_TRANSFORM_CHANGED);
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
	this->emit(NodeEvent::ON_TRANSFORM_CHANGED);
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
    this->_uiWorldMatrix.setM12(this->_uiWorldMatrix.getM30() + (0.5 - this->_anchor.getX()) * this->_size.getWidth());
    this->_uiWorldMatrix.setM13(this->_uiWorldMatrix.getM31() + (0.5 - this->_anchor.getY()) * this->_size.getHeight());
}
void Node2D::update(float dt)
{
	Node::update(dt);
	std::cout << "Node update: " << _name << std::endl;
}
void Node2D::lateUpdate(float dt)
{
	Node::lateUpdate(dt);
	std::cout << "Node lateUpdate: " << _name << std::endl;
}
void Node2D::render()
{
	Node::render();
	if (_active)
	{
		std::cout << "Node render: " << _name << std::endl;
	}
}