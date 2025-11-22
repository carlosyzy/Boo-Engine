#include "component.h"
#include "../scene/node.h"
#include "../utils/uuid-util.h"
#include "../../boo.h"

Component::Component(Node *node, std::string uuid) : _layer(NodeLayer::Node)
{
    this->_uuid = uuid;
    if (this->_uuid.size() <= 0)
    {
        this->_uuid = UuidUtil::generateUUID();
    }
    this->_node = node;
}
/**
 * @brief 组件唤醒函数
 * 组件被添加到节点后,第一次激活会调用Awake函数
 */
void Component::Awake(){

}
/**
 * @brief 组件启用函数
 * 组件被激活后,会调用Enable函数
 */
void Component::Enable(){

}

void Component::Update(float deltaTime)
{
    if (!this->isEnabled())
        return; // 组件未激活
}
void Component::LateUpdate(float deltaTime)
{
    if (!this->isEnabled())
        return; // 组件未激活
}
void Component::Render()
{
    if (!this->isEnabled())
        return; // 组件未激活
}
void Component::LateRender()
{
    if (!this->isEnabled())
        return; // 组件未激活
}
/**
 * @brief 组件禁用函数
 * 组件被禁用后,会调用Disable函数
 */
void Component::Disable(){

}



void Component::destroy()
{
    std::cout << "Component::destroy" << std::endl;
    Boo::game->addCompClearCaches(this);
}
Component::~Component()
{
    std::cout << "Component::~destructor" << std::endl;
}
