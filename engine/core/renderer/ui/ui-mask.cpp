#include "ui-mask.h"
#include "../../../boo.h"
#include "../../gfx/gfx-mgr.h"
#include "../../scene/node.h"
#include "../../scene/node-2d.h"
#include "../../assets/texture.h"

UIMask::UIMask(Node *node, std::string uuid) : Component(node, uuid)
{
    this->_name = "UIMask";
    // 默认裁剪区域为整个UI对象
    // GfxMgr::getInstance()->createUIObject(this->_uuid, this->_positions, this->_colors, this->_normals, this->_uvs, this->_indices);
    // GfxMgr::getInstance()->setObjectPass(this->_uuid, "ui");
    // GfxMgr::getInstance()->setObjectPipeline(this->_uuid, "ui.mtl");
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
    // 提交遮罩状态到渲染器
    // GfxMgr::getInstance()->submitObjectRender(this->_uuid);
}
void UIMask::lateRender()
{
    Component::lateRender();
    // GfxMgr::getInstance()->submitObjectRender(this->_uuid);
}
void UIMask::destroy()
{
    Component::destroy();
}
UIMask::~UIMask()
{
}
