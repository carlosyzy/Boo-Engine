#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include "component.h"
#include "../scene/node.h"

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
    template <typename T = Component>
    T *create(const std::string &className, Node *node, std::string uuid)
    {
        auto it = this->_creators.find(className);
        if (it != this->_creators.end())
        {
            Component *component = it->second(node, uuid);
            T *result = dynamic_cast<T *>(component);
            if (result)
            {
                return result;
            }
            else
            {
                std::cout << "Dynamic cast failed for: " << className << std::endl;
                delete component; // 清理内存
                return nullptr;
            }
            return result;
        }
        return nullptr;
    }
};
