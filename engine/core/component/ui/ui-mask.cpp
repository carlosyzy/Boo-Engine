#include "ui-mask.h"
#include "../../../boo.h"
#include "../../gfx/gfx-mgr.h"
#include "../../scene/node.h"
#include "../../scene/node-2d.h"
#include "../../assets/texture.h"

UIMask::UIMask(Node *node, std::string uuid) : Component(node, uuid)
{
    this->_name = "UIMask";
    //默认裁剪区域为整个UI对象
    GfxMgr::getInstance()->addUIObjectMask(this->_node->getUuid(), this->_uuid, {0.0f, 0.0f, 1.0f, 1.0f, 0.0f});
}
void UIMask::update(float deltaTime)
{
    Component::update(deltaTime);
}
void UIMask::lateUpdate(float deltaTime)
{
    Component::lateUpdate(deltaTime);
}
void UIMask::render()
{
    Component::render();
    //提交遮罩状态到渲染器
    // GfxMgr::getInstance()->setMask(this->_node->getGlobalPosition(), this->_node->getGlobalScale());

}
void UIMask::destroy()
{
    Component::destroy();
}
UIMask::~UIMask()
{
}
