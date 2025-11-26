#pragma once
#include "component-factory.h"
#include "component.h"
#include "../scene/node.h"

template <typename Component>
bool ComponentFactory::registerComponent(const std::string &name, const std::string &description)
{
    return this->registerComponent(name, [](std::string name, Node *node, std::string uuid) -> Component *
                                   { return new Component(name, node, uuid); }, description);
};