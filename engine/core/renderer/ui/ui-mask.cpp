#include "ui-mask.h"
#include "../../../boo.h"
#include "../../gfx/gfx-mgr.h"
#include "../../scene/node.h"
#include "../../scene/node-2d.h"
#include "../../assets/texture.h"
#include "../../assets/assets-manager.h"

UIMask::UIMask(Node *node, std::string uuid) : Component(node, uuid)
{
    this->_name = "UIMask";
    this->_layer = NodeLayer::Node2D;
    this->_addUuid = this->_uuid + "-add";
    this->_subUuid = this->_uuid + "-sub";
    // 默认裁剪区域为整个UI对象
    GfxMgr::getInstance()->createUIMaskObject(this->_addUuid, this->_positions, this->_colors, this->_normals, this->_uvs, this->_indices);
    GfxMgr::getInstance()->createUIMaskObject(this->_subUuid, this->_positions, this->_colors, this->_normals, this->_uvs, this->_indices);
    GfxMgr::getInstance()->setObjectPass(this->_addUuid, "built-ui");
    GfxMgr::getInstance()->setObjectPass(this->_subUuid, "built-ui");
    GfxMgr::getInstance()->setObjectPipeline(this->_addUuid, "built-ui-mask-add");
    GfxMgr::getInstance()->setObjectPipeline(this->_subUuid, "built-ui-mask-sub");
    GfxMgr::getInstance()->setObjectUIMaskBehavior(this->_addUuid, 1);
    GfxMgr::getInstance()->setObjectUIMaskBehavior(this->_subUuid, 0);
    this->setTexture("resources/texture/ic-default.png");
}
void UIMask::updateModelMatrix()
{
	Node2D *node2D = dynamic_cast<Node2D *>(this->_node);
	GfxMgr::getInstance()->setObjectModelMatrix(this->_addUuid, node2D->uiWorldMatrix().data());
    GfxMgr::getInstance()->setObjectModelMatrix(this->_subUuid, node2D->uiWorldMatrix().data());
}
void UIMask::setTexture(Texture *texture)
{
	if (texture == nullptr)
	{
		std::cout << "UIRenderer::setTexture: texture is nullptr" << std::endl;
		return;
	}
	if (this->_texture == texture)
	{
		return;
	}
	this->_texture = texture;
	GfxMgr::getInstance()->setObjectTexture(this->_addUuid, this->_texture->getUuid());
    GfxMgr::getInstance()->setObjectTexture(this->_subUuid, this->_texture->getUuid());
}
void UIMask::setTexture(std::string texture)
{
	Asset *tex = Boo::game->assetsManager()->get(texture);
	this->setTexture(dynamic_cast<Texture *>(tex));
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
    if (this->_node->hasFrameTransformFlag())
    {
        this->updateModelMatrix();
    }
    Component::render();
    // 提交遮罩状态到渲染器
    // std::cout << "UIMask::render " << this->_addUuid << std::endl;
    // GfxMgr::getInstance()->setObjectUIMaskBehavior(this->_addUuid, 1);
    GfxMgr::getInstance()->submitObjectRender(this->_addUuid);
}
void UIMask::lateRender()
{
    Component::lateRender();
    // 提交遮罩状态到渲染器
    // std::cout << "UIMask::lateRender" << std::endl;
    // GfxMgr::getInstance()->setObjectUIMaskBehavior(this->_subUuid, 0);
    GfxMgr::getInstance()->submitObjectRender(this->_subUuid);
}
void UIMask::destroy()
{
    Component::destroy();
}
UIMask::~UIMask()
{
}
