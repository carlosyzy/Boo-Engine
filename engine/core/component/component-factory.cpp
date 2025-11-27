#include "component-factory.h"
/**
 * @brief 获取组件工厂实例
 * 
 * @return ComponentFactory& 组件工厂实例(指针)
 */
ComponentFactory &ComponentFactory::getInstance()
{
    static ComponentFactory instance;
    return instance;
}
bool ComponentFactory::registerComponent(const std::string &name, std::function<Component *(std::string name, Node *node, std::string uuid)> creator,std::string description) {
    if (name.empty()) {
        std::cerr << "❌ 组件类型名称不能为空" << std::endl;
        return false;
    }
    
    if (this->_creators.find(name) != this->_creators.end()) {
        // std::cerr << "❌ 组件 '" << name << "' 已经注册" << std::endl;
        return false;
    }
    
    this->_creators[name] = creator;
    this->_descriptions[name] = description;
    std::cout << "✅ 注册组件: " << name;
    if (!description.empty()) {
        std::cout << " - " << description;
    }
    std::cout << std::endl;
    return true;
}
/**
 * @brief 创建组件实例
 * 
 * @param name 组件名称
 * @return Component* 组件实例指针
 */
Component* ComponentFactory::createComponent(const std::string &name, Node *node, std::string uuid) {
    auto it = this->_creators.find(name);
    if (it != this->_creators.end()) {
        Component* component = it->second(name, node, uuid);
        return component;
    }
    std::cerr << "❌ 未知组件类型: " << name << std::endl;
    return nullptr;
}
bool ComponentFactory::isComponentRegistered(const std::string& typeName) const {
    return this->_creators.find(typeName) != this->_creators.end();
}
std::string ComponentFactory::getComponentDescription(const std::string &name) {
   /* auto it = this->_descriptions.find(name);
    return it != this->_descriptions.end() ? it->second : "无描述";*/
    return "";
}
