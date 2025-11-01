#include "scene.h"
#include <iostream>

Scene::Scene(const std::string name, const std::string uuid) : Node(name, uuid) {
    
}

Scene::~Scene() {
    
}

void Scene::update(float dt) {
	Node::update(dt);
	// 基础更新逻辑，子类可以重写
	std::cout << "Node update: " << _name << std::endl;
}
void Scene::lateUpdate(float dt) {
	Node::lateUpdate(dt);
	// 基础更新逻辑，子类可以重写
	std::cout << "Node lateUpdate: " << _name << std::endl;
}
void Scene::render() {
    Node::render();
}