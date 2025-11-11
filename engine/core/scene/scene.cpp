#include "scene.h"
#include <iostream>
#include "../utils/uuid-util.h"
#include "node-3d.h"
#include "node-2d.h"

Scene::Scene(const std::string name, const std::string uuid)
{
	this->_name = name;
	this->_layer = NodeLayer::Scene;
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
	this->_root3D = new Node3D("root3D");
	this->_root2D = new Node2D("root2D");
	this->addChild(this->_root3D);
	this->addChild(this->_root2D);
}

Component *Scene::addComponent(std::string name, std::string uuid)
{
	return nullptr;
}

Component *Scene::getComponent(std::string name)
{
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

void Scene::render()
{
	Node::render();
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
}