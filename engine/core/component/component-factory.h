#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include "component.h"
#include "../scene/node.h"
#include "../scene/node-2d.h"
#include "../scene/node-3d.h"

class ComponentFactory
{
private:
    std::unordered_map<std::string, std::function<Component *(Node *, std::string)>> _creators;

public:
    ComponentFactory();
    ~ComponentFactory();

    template <typename T = Component>
    void registerClass(const std::string &className)
    {
        static_assert(std::is_base_of<Component, T>::value,
                      "T must be derived from Component");
        this->_creators[className] = [](Node *node, std::string uuid) -> Component *
        {
            return new T(node, uuid);
        };
    }
   
    Component *create(const std::string &className, Node *node, std::string uuid)
    {
        auto it = this->_creators.find(className);
        if (it != this->_creators.end())
        {
            return it->second(node, uuid);
        }

        std::cout << "Component class not found: " << className << std::endl;
        return nullptr;
    }
};
