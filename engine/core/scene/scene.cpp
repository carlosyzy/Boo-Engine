#include "scene.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <sstream>
#include <iomanip>

Scene::Scene(const std::string& name, const std::string& uuid) :NodeBase(name,uuid) {
    
}

Scene::~Scene() {
}

void Scene::update() {
    std::cout << "Scene update: " << _name << std::endl;
   
}

void Scene::render() {
    std::cout << "Scene render: " << _name << std::endl;
   
}