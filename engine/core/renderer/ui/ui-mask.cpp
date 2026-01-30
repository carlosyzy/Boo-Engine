#include "ui-mask.h"
#include "../../../boo.h"
#include "../../gfx/gfx-mgr.h"
#include "../../gfx/base/gfx-mesh.h"
#include "../../scene/node.h"
#include "../../scene/node-2d.h"
#include "../../math/size.h"
#include "../../assets/texture-asset.h"
#include "../../assets/material-asset.h"
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
    // this->_addUuid = this->_uuid + "-add";
    // this->_subUuid = this->_uuid + "-sub";

    // // 默认裁剪区域为整个UI对象
    // GfxMgr::getInstance()->createUIMaskObject(this->_addUuid, this->_positions, this->_colors, this->_normals, this->_uvs, this->_indices);
    // GfxMgr::getInstance()->createUIMaskObject(this->_subUuid, this->_positions, this->_colors, this->_normals, this->_uvs, this->_indices);
    // GfxMgr::getInstance()->setObjectPass(this->_addUuid, "built-ui");
    // GfxMgr::getInstance()->setObjectPass(this->_subUuid, "built-ui");
    // GfxMgr::getInstance()->setObjectPipeline(this->_addUuid, "built-ui-mask-add");
    // GfxMgr::getInstance()->setObjectPipeline(this->_subUuid, "built-ui-mask-sub");
    // GfxMgr::getInstance()->setObjectUIMaskBehavior(this->_addUuid, 1);
    // GfxMgr::getInstance()->setObjectUIMaskBehavior(this->_subUuid, 0);
    // this->setTexture("resources/texture/ic-default.png");

    this->_addMaterialAsset = new MaterialAsset();
    this->_subMaterialAsset = new MaterialAsset();

    // this->_addMaterialAsset->createUIMaskTest(0);
    // this->_subMaterialAsset->createUIMaskTest(1);

    // this->_testMaterialAsset = new MaterialAsset();
    // this->_testMaterialAsset->createMaskUITest();

    std::string meshUuid = this->_uuid + "-mask";
    this->_maskMesh = new GfxMesh(meshUuid);
}
void UIMask::_deserialized()
{
    Component::_deserialized();
}

void UIMask::Awake()
{
    Component::Awake();
    // std::cout << "=================ui mask awake" << std::endl;
}
void UIMask::Enable()
{
    Component::Enable();
    // std::cout << "=================ui mask enable" << std::endl;
    this->_setColor(1.0, 1.0, 1.0, 1.0);
    this->_updateNodeMask();
}
void UIMask::_updateNodeMask()
{
    Node2D *node2D = dynamic_cast<Node2D *>(this->_node);
    if (node2D == nullptr)
    {
        std::cout << "UIMask::_updateNodeMask: node is not Node2D" << std::endl;
        return;
    }
    const Size &size = node2D->getSize();
    const Mat4 &worldMatrix = node2D->getWorldMatrix();
    Vec3 leftTop = Vec3(-size.getWidth() / 2.0f, size.getHeight() / 2.0f, 0.0f);
    Vec3 leftBottom = Vec3(-size.getWidth() / 2.0f, -size.getHeight() / 2.0f, 0.0f);
    Vec3 rightBottom = Vec3(size.getWidth() / 2.0f, -size.getHeight() / 2.0f, 0.0f);
    Vec3 rightTop = Vec3(size.getWidth() / 2.0f, size.getHeight() / 2.0f, 0.0f);

    Mat4::multiplyVec3(worldMatrix, leftTop, leftTop);
    Mat4::multiplyVec3(worldMatrix, leftBottom, leftBottom);
    Mat4::multiplyVec3(worldMatrix, rightBottom, rightBottom);
    Mat4::multiplyVec3(worldMatrix, rightTop, rightTop);

    // std::cout << "leftTop: " << leftTop.getX() << ", " << leftTop.getY() << ", " << leftTop.getZ() << std::endl;
    // std::cout << "leftBottom: " << leftBottom.getX() << ", " << leftBottom.getY() << ", " << leftBottom.getZ() << std::endl;
    // std::cout << "rightBottom: " << rightBottom.getX() << ", " << rightBottom.getY() << ", " << rightBottom.getZ() << std::endl;
    // std::cout << "rightTop: " << rightTop.getX() << ", " << rightTop.getY() << ", " << rightTop.getZ() << std::endl;

    std::vector<float> positions = {
        leftTop.getX(), leftTop.getY(), 0.0f, 0.0f, 0.0f,
        leftBottom.getX(), leftBottom.getY(), 0.0f, 0.0f, 1.0f,
        rightBottom.getX(), rightBottom.getY(), 0.0f, 1.0f, 1.0f,
        rightTop.getX(), rightTop.getY(), 0.0f, 1.0f, 0.0f};
    std::vector<uint32_t> indices = {
        0, 1, 2,
        0, 2, 3};
    this->_maskMesh->setInputVertices(positions, indices);
}
void UIMask::Update(float deltaTime)
{
    Component::Update(deltaTime);
}
void UIMask::LateUpdate(float deltaTime)
{
    Component::LateUpdate(deltaTime);
    if (this->_node->hasFrameTransformFlag())
    {
        this->_updateNodeMask();
    }
}
void UIMask::Render(Camera *camera)
{
    if (camera == nullptr)
    {
        return; // 相机为空
    }
    Node2D *node2D = dynamic_cast<Node2D *>(this->_node);
    if (node2D == nullptr)
    {
        return; // 节点不是Node2D类型
    }
    if (node2D->getSize().getHeight() <= 0 || node2D->getSize().getWidth() <= 0)
    {
        return; // 节点不可见
    }

    // 提交渲染对象
    this->_instanceData.clear();
    this->_instanceData.reserve(16 + 4);
    // 1. 先添加模型矩阵 (16个float)
    const auto &matrix = node2D->getWorldMatrix().data();
    _instanceData.insert(_instanceData.end(), matrix.begin(), matrix.end());
    // 2. 再添加颜色 (4个float)
    _instanceData.insert(_instanceData.end(), {1.0f, 1.0f, 1.0f, 1.0f});

    GfxMgr::getInstance()->submitRenderObject(camera->getUuid(), this->_addMaterialAsset->getGfxMaterial(), this->_maskMesh, this->_instanceData);
    // GfxMgr::getInstance()->submitRenderObject(camera->getUuid(), this->_testMaterialAsset->getGfxMaterial(), this->_maskMesh, this->_instanceData);
}
void UIMask::lateRender(Camera *camera)
{
    if (camera == nullptr)
    {
        return; // 相机为空
    }
    Node2D *node2D = dynamic_cast<Node2D *>(this->_node);
    if (node2D == nullptr)
    {
        return; // 节点不是Node2D类型
    }
    if (node2D->getSize().getHeight() <= 0 || node2D->getSize().getWidth() <= 0)
    {
        return; // 节点不可见
    }

    // 提交渲染对象
    this->_instanceData.clear();
    this->_instanceData.reserve(16 + 4);
    // 1. 先添加模型矩阵 (16个float)
    const auto &matrix = node2D->getWorldMatrix().data();
    _instanceData.insert(_instanceData.end(), matrix.begin(), matrix.end());
    // 2. 再添加颜色 (4个float)
    _instanceData.insert(_instanceData.end(), {1.0f, 1.0f, 1.0f, 1.0f});

    GfxMgr::getInstance()->submitRenderObject(camera->getUuid(), this->_subMaterialAsset->getGfxMaterial(), this->_maskMesh, this->_instanceData);
    // GfxMgr::getInstance()->submitRenderObject(camera->getUuid(), this->_testMaterialAsset->getGfxMaterial(), this->_maskMesh, this->_instanceData);
}
void UIMask::Disable()
{
    Component::Disable();
}
void UIMask::destroy()
{
    Component::destroy();
}
UIMask::~UIMask()
{
}
