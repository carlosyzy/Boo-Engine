#include "component.h"
#include "../scene/node.h"
#include "../utils/uuid-util.h"
#include "../../boo.h"

Component::Component(std::string name, Node *node, std::string uuid) : _layer(NodeLayer::Default)
{
    this->_uuid = uuid;
    if (this->_uuid.size() <= 0)
    {
        this->_uuid = UuidUtil::generateUUID();
    }
    this->_name = name;
    this->_node = node;
    this->_isEnabled = true;
    this->_isEnabledInHierarchy = false;
    this->_isAwaked = false;
}
// 序列化组件属性-配置
void Component::_deserialized()
{
    
}

void Component::setEnabled(bool enabled)
{
    this->_isEnabled = enabled;
    this->setNodeActiveInHierarchy(this->_node->isActiveInHierarchy());
}

void Component::setNodeActiveInHierarchy(bool isActiveInHierarchy)
{
    bool isEnableInHierarchy = isActiveInHierarchy && this->_isEnabled;
    if (this->_isEnabledInHierarchy == isEnableInHierarchy)
    {
        return; // 状态未改变
    }
    this->_isEnabledInHierarchy = isEnableInHierarchy;
    // std::cout << "Component::setNodeActiveInHierarchy: " << this->_name << " isEnabledInHierarchy: " << this->_isEnabledInHierarchy << std::endl;
    if (this->_isEnabledInHierarchy)
    {
        if (!this->_isAwaked)
        {
            this->_isAwaked = true;
            this->Awake();
        }
        this->Enable();
    }
    else
    {
        this->Disable();
    }
}
/**
 * @brief 组件唤醒函数
 * 组件被添加到节点后,第一次激活会调用Awake函数
 */
void Component::Awake()
{
    // std::cout << "Component::Awake" << std::endl;
}
/**
 * @brief 组件启用函数
 * 组件被激活后,会调用Enable函数
 */
void Component::Enable()
{
    // std::cout << "Component::Enable" << std::endl;
}

void Component::Update(float deltaTime)
{
   
}
void Component::LateUpdate(float deltaTime)
{
   
}
/**
 * @brief 组件禁用函数
 * 组件被禁用后,会调用Disable函数
 */
void Component::Disable()
{
    // std::cout << "Component::Disable" << std::endl;
}

void Component::destroy()
{
    // std::cout << "Component::destroy" << std::endl;
    Boo::game->addCompClearCaches(this);
}

Component::~Component()
{
    // std::cout << "Component::~destructor" << std::endl;
}
