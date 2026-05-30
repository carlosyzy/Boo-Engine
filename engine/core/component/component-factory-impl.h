#pragma once
#include "core/component/component-factory.h"
#include "core/component/component.h"
#include "core/scene/node.h"
namespace Boo
{
    template <typename Component>
    bool ComponentFactory::registerComponent(const std::string &name, const std::string &description)
    {
        return this->registerComponent(name, [](std::string name, Node *node, std::string uuid) -> Component *
                                       { return new Component(name, node, uuid); }, description);
    };
}