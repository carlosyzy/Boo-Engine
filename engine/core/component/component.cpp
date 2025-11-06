#include "component.h"
#include "../scene/node.h"
#include "../utils/uuid-util.h"

Component::Component(Node *node, std::string uuid) : _layer(NodeLayer::Node)
{
    this->_uuid = uuid;
    if (this->_uuid.size() <= 0)
    {
        this->_uuid = UuidUtil::generateUUID();
    }
    this->_node = node;
}

void Component::update(float deltaTime)
{
    if (!this->isEnabled())
        return; // 组件未激活
}
void Component::lateUpdate(float deltaTime)
{
    if (!this->isEnabled())
        return; // 组件未激活
}
void Component::render()
{
    if (!this->isEnabled())
        return; // 组件未激活
}

void Component::destroy()
{
    
}
Component::~Component() {

}
