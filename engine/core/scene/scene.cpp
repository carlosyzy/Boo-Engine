#include "scene.h"
#include <iostream>
#include "../utils/uuid-util.h"

Scene::Scene(const std::string name, const std::string uuid)
{
	this->_name = name;
	this->_layer = NodeLayer::Scene;
	this->_visibility = uint32_t(NodeVisibility::Default);
	this->_uuid = uuid.empty() ? UuidUtil::generateUUID() : uuid;
	this->_active = false;
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
}
void Scene::createRoot2D()
{
	if (this->_root2D != nullptr)
	{
		return;
	}
	this->_root2D = new Node2D("root2D");
	this->_root2D->_isLocked = true;
	this->_root2D->setVisibility(uint32_t(NodeVisibility::Node2D));
	this->addChild(this->_root2D);
	// Boo::game->input()->setRoot2D(this->_root2D);
}
void Scene::createRoot3D()
{
	if (this->_root3D != nullptr)
	{
		return;
	}
	this->_root3D = new Node3D("root3D");
	this->_root3D->_isLocked = true;
	this->_root3D->setVisibility(uint32_t(NodeVisibility::Node3D));
	this->addChild(this->_root3D);
}
Node3D *Scene::getRoot3D()
{
	if (this->_root3D == nullptr)
	{
		std::cout << "Scene::getRoot3D: root3D is null" << std::endl;
		return nullptr;
	}
	return this->_root3D;
}
Node2D *Scene::getRoot2D()
{
	if (this->_root2D == nullptr)
	{
		std::cout << "Scene::getRoot2D: root2D is null" << std::endl;
		return nullptr;
	}
	return this->_root2D;
}

void Scene::setActive(bool active)
{
	this->_active = active;
	bool _isActiveInHierarchy = this->_active;
	this->_updateNodesActiveInHierarchyState(_isActiveInHierarchy);
}
Component *Scene::addComponent(std::string name, std::string uuid)
{
	std::cout << name << ":Component add fail,node type is Scene" << std::endl;
	return nullptr;
}
void Scene::update(float dt)
{
	Node::update(dt);
}
void Scene::lateUpdate(float dt)
{
	Node::lateUpdate(dt);
}
void Scene::clearNodeFrameFlag()
{
	Node::clearNodeFrameFlag();
}
void Scene::destroy()
{
	Node::destroy();
}
Scene::~Scene()
{
	std::cout << "Scene::~destructor" << std::endl;
}