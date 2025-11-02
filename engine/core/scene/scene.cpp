#include "scene.h"
#include <iostream>
#include "node-3d.h"
#include "node-2d.h"



Scene::Scene(const std::string name, const std::string uuid) : Node(name, uuid) {
    this->_layer = NodeLayer::Scene;
    this->_root3D = new Node3D("root3D");
    this->_root2D = new Node2D("root2D");
	this->addChild(this->_root3D);
	this->addChild(this->_root2D);
}


Component* Scene::addComponent(std::string name, std::string uuid) {
	return nullptr;
}

Component* Scene::getComponent(std::string name) {
	return nullptr;
}
void Scene::update(float dt) {
	Node::update(dt);
}
void Scene::lateUpdate(float dt) {
	Node::lateUpdate(dt);
}

void Scene::render() {
    Node::render();
}
void Scene::clearNodeFrameFlag() {
	Node::clearNodeFrameFlag();
}
void Scene::destroy() {
	Node::destroy();
}
Scene::~Scene() {
	
}