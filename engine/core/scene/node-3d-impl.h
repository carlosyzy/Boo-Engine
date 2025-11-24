#pragma once
#include "node-2d.h"
#include "../../boo.h"
#include "../input/input.h"
#include "../../core/component/component.h"

// template <typename Component>
// Component *Node3D::addComponent(std::string name, std::string uuid)
// {
//     Component *component = new Component(name, this, uuid);
//     this->_components.push_back(component);
//     if (this->_parent != nullptr)
//     {
//         component->setNodeActiveInHierarchy(this->_isActiveInHierarchy);
//     }
//     return component;
// }