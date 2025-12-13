#include "ui-mask.h"
#include "../../../boo.h"
#include "../../gfx/gfx-mgr.h"
#include "../../scene/node.h"
#include "../../scene/node-2d.h"
#include "../../assets/texture-asset.h"
#include "../../assets/assets-manager.h"
#include "../../renderer/camera.h"
UIMask::UIMask(std::string name, Node *node, std::string uuid) : UIRenderer(name, node, uuid)
{
    // this->_positions = {
    //     -0.5f, 0.5f, 0.0f,  /** @brief 左上 */
    //     -0.5f, -0.5f, 0.0f, /** @brief 坐下 */
    //     0.5f, -0.5f, 0.0f,  /** @brief 右下 */
    //     0.5f, 0.5f, 0.0f    /** @brief 右上 */
    // };
    // this->_colors = {
    //     1.0f, 1.0f, 1.0f, 1.0f,
    //     1.0f, 1.0f, 1.0f, 1.0f,
    //     1.0f, 1.0f, 1.0f, 1.0f,
    //     1.0f, 1.0f, 1.0f, 1.0f};
    // this->_normals = {
    //     0.0f, 0.0f, 0.0f,
    //     0.0f, 0.0f, 0.0f,
    //     0.0f, 0.0f, 0.0f,
    //     0.0f, 0.0f, 0.0f};
    // this->_uvs = {
    //     0.0f, 0.0f,
    //     0.0f, 1.0f,
    //     1.0f, 1.0f,
    //     1.0f, 0.0f};
    // this->_indices = {
    //     0, 1, 2,
    //     0, 2, 3};
    this->_addUuid = this->_uuid + "-add";
    this->_subUuid = this->_uuid + "-sub";

    // // 默认裁剪区域为整个UI对象
    // GfxMgr::getInstance()->createUIMaskObject(this->_addUuid, this->_positions, this->_colors, this->_normals, this->_uvs, this->_indices);
    // GfxMgr::getInstance()->createUIMaskObject(this->_subUuid, this->_positions, this->_colors, this->_normals, this->_uvs, this->_indices);
    // GfxMgr::getInstance()->setObjectPass(this->_addUuid, "built-ui");
    // GfxMgr::getInstance()->setObjectPass(this->_subUuid, "built-ui");
    // GfxMgr::getInstance()->setObjectPipeline(this->_addUuid, "built-ui-mask-add");
    // GfxMgr::getInstance()->setObjectPipeline(this->_subUuid, "built-ui-mask-sub");
    // GfxMgr::getInstance()->setObjectUIMaskBehavior(this->_addUuid, 1);
    // GfxMgr::getInstance()->setObjectUIMaskBehavior(this->_subUuid, 0);
    // this->setTextureAsset("resources/texture/ic-default.png");
}
void UIMask::_deserialized()
{
    Component::_deserialized();
}

void UIMask::Awake()
{
    Component::Awake();
}
void UIMask::Enable()
{
    Component::Enable();
    // GfxMgr::getInstance()->createUIMaskObject(this->_addUuid, this->_positions, this->_colors, this->_normals, this->_uvs, this->_indices);
    // GfxMgr::getInstance()->createUIMaskObject(this->_subUuid, this->_positions, this->_colors, this->_normals, this->_uvs, this->_indices);
    // this->_updateRendererState();
    // this->_updateModelMatrix();
}
void UIMask::setMaterialAsset(std::string mtl)
{
    // MaterialAsset *mtlAsset = dynamic_cast<MaterialAsset *>(Boo::game->assetsManager()->get(mtl));
    // if (mtlAsset == nullptr)
    // {
    //     std::cout << "UIMask::setMaterialAsset: material " << mtl << " not found" << std::endl;
    //     return;
    // }
    // this->_setMaterial(mtlAsset);
}
void UIMask::setMaterialAsset(MaterialAsset *mtl)
{
    if (mtl == nullptr)
    {
        std::cout << "UIMask::setMaterialAsset: material is nullptr" << std::endl;
        return;
    }
    this->_setMaterial(mtl);
}

// void UIMask::_updateRendererState()
// {
// }
// void UIMask::_updateModelMatrix()
// {
//     Node2D *node2D = dynamic_cast<Node2D *>(this->_node);
//     // GfxMgr::getInstance()->setObjectModelMatrix(this->_addUuid, node2D->uiWorldMatrix().data());
//     // GfxMgr::getInstance()->setObjectModelMatrix(this->_subUuid, node2D->uiWorldMatrix().data());
// }

void UIMask::Update(float deltaTime)
{
    Component::Update(deltaTime);
}
void UIMask::LateUpdate(float deltaTime)
{
    Component::LateUpdate(deltaTime);
}
void UIMask::Render(Camera *camera)
{
    // if (this->_node->hasFrameTransformFlag())
    // {
    //     this->_updateModelMatrix();
    // }
    // 提交遮罩状态到渲染器
    // GfxMgr::getInstance()->submitObjectRender(this->_addUuid);
}
void UIMask::lateRender()
{
    // GfxMgr::getInstance()->submitObjectRender(this->_subUuid);
}
void UIMask::Disable()
{
    Component::Disable();
}
void UIMask::destroy()
{
    Component::destroy();
    // GfxMgr::getInstance()->destroyObject(this->_addUuid);
    // GfxMgr::getInstance()->destroyObject(this->_subUuid);
}
UIMask::~UIMask()
{
}
