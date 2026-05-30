#include "node-3d.h"
#include <iostream>
#include "boo.h"
#include "log.h"
#include "core/util/uuid-util.h"
#include "core/component/component-factory.h"
#include "core/renderer/3d/mesh-renderer.h"

namespace Boo
{

	Node3D::Node3D(const std::string name, const std::string uuid)
	{
		this->_groupID = uint32_t(ENodeGroup::Node3D);
		this->_name = name;
		this->_layer = ENodeLayer::Node3D;
		this->_uuid = uuid.empty() ? UuidUtil::generateUUID() : uuid;
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
		this->_worldTransformFlag = static_cast<uint32_t>(ENodeTransformFlag::ALL_FLAG);
		this->_frameTransformFlag = static_cast<uint32_t>(ENodeTransformFlag::ALL_FLAG);
		this->_parent = nullptr;
		this->_meshRenderer = nullptr;
	}
	void Node3D::setPosition(float x, float y, float z)
	{
		Node::setPosition(x, y, z);
	}
	void Node3D::setWorldPosition(float x, float y, float z)
	{
		Node::setWorldPosition(x, y, z);
	}
	void Node3D::setScale(float x, float y, float z)
	{
		Node::setScale(x, y, z);
	}
	void Node3D::setWorldScale(float x, float y, float z)
	{
		Node::setWorldScale(x, y, z);
	}
	void Node3D::setEulerAngles(float x, float y, float z)
	{
		Node::setEulerAngles(x, y, z);
	}
	void Node3D::setRotation(float x, float y, float z, float w)
	{
		Node::setRotation(x, y, z, w);
	}
	void Node3D::setWorldRotation(float x, float y, float z, float w)
	{
		Node::setWorldRotation(x, y, z, w);
	}
	void Node3D::_updateWorldTransform()
	{
		if (!this->_isActiveInHierarchy)
			return;
		if (this->_worldTransformFlag == ENodeTransformFlag::NONE_FLAG)
			return;
		Node::_updateWorldTransform();
		// 计算世界矩阵的逆转置矩阵
		Mat4::inverseTranspose(this->_worldMatrix, this->_worldMatrixIT);
	}
	/**
	 * @brief 获取世界矩阵的逆转置矩阵
	 * @return Mat4 世界矩阵的逆转置矩阵
	 */
	const Mat4 &Node3D::getWorldMatrixIT()
	{
		this->_updateWorldTransform();
		return this->_worldMatrixIT;
	}
	void Node3D::setActive(bool active)
	{
		Node::setActive(active);
	}

	Component *Node3D::addComponent(std::string name, std::string uuid)
	{
		Component *component = ComponentFactory::getInstance().createComponent(name, this, uuid);
		if (component == nullptr)
		{
			// std::cout << name << ":Component Not register" << std::endl;
			LOGW("[Node2D]:addComponent:: %s, %s, Component Not register", name.c_str(), uuid.c_str());
			return nullptr;
		}
		if (component->getLayer() == EComponentLayer::Layer2D)
		{
			// std::cout << name << ":Component add fail,node type is Node2D" << std::endl;
			LOGW("[Node3D]:addComponent:: %s, %s, Component add fail,node type is Node2D", name.c_str(), uuid.c_str());
			delete component;
			component = nullptr;
			return nullptr;
		}
		if (dynamic_cast<MeshRenderer *>(component) != nullptr)
		{
			if (this->_meshRenderer == nullptr)
			{
				this->_meshRenderer = dynamic_cast<MeshRenderer *>(component);
			}
			else
			{
				LOGW("[Node3D]:addComponent:: %s, %s, Component add fail,node already has MeshRenderer", name.c_str(), uuid.c_str());
				delete component;
				component = nullptr;
				return nullptr;
			}
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
	MeshRenderer *Node3D::getMeshRenderer()
	{
		return this->_meshRenderer;
	}
	void Node3D::update(float dt)
	{
		Node::update(dt);
	}
	void Node3D::lateUpdate(float dt)
	{
		Node::lateUpdate(dt);
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
		// std::cout << "Node3D::~destructor: " << this->_name << std::endl;
	}

} // namespace Boo