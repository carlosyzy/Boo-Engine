#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <functional>
#include "component.h"

class ComponentFactory
{
private:
    ComponentFactory() = default;
    ~ComponentFactory() = default;
    // 禁用拷贝和赋值
    ComponentFactory(const ComponentFactory &) = delete;
    ComponentFactory &operator=(const ComponentFactory &) = delete;
    ComponentFactory(ComponentFactory &&) = delete;
    ComponentFactory &operator=(ComponentFactory &&) = delete;

    // 存储组件创建函数和相关信息
    std::unordered_map<std::string, std::function<Component *(std::string name, Node *node, std::string uuid)>> _creators;
    std::unordered_map<std::string, std::string> _descriptions;

public:
    static ComponentFactory &getInstance();

     // 方法1：模板注册函数
    template <typename Component>
    bool registerComponent(const std::string &name, const std::string &description = "");
    /**
     * @brief 注册组件
     *
     * @param name 组件名称
     * @param component 组件实例(指针)
     * @param description 组件描述
     */
    bool registerComponent(const std::string &name, std::function<Component *(std::string name, Node *node, std::string uuid)> creator, std::string description = "");

    /**
     * @brief 创建组件实例
     *
     * @param name 组件名称
     * @return Component* 组件实例指针
     */
    Component *createComponent(const std::string &name, Node *node, std::string uuid);
    /**
     * @brief 获取组件描述
     *
     * @param name 组件名称
     * @return std::string 组件描述
     */
    std::string getComponentDescription(const std::string &name);

    bool isComponentRegistered(const std::string &typeName) const;
};
#include "component-factory-impl.h"
