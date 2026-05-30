#include "ui-mask.h"
#include "boo.h"
#include "core/gfx/gfx-manager.h"
#include "core/gfx/base/gfx-mesh.h"
#include "core/scene/node.h"
#include "core/scene/node-2d.h"
#include "core/asset/texture-asset.h"
#include "core/asset/material-asset.h"
#include "core/asset/mesh-asset.h"
#include "core/asset/asset-manager.h"
#include "core/renderer/camera.h"

namespace Boo
{

    UIMask::UIMask(std::string name, Node *node, std::string uuid) : UIRenderer(name, node, uuid),
                                                                     _maskMesh(nullptr),
                                                                     _addMaterialAsset(nullptr),
                                                                     _subMaterialAsset(nullptr)
    {
        // 默认纹理
        this->_addMaterialAsset = nullptr;
        this->_subMaterialAsset = nullptr;
        this->_maskMesh = nullptr;
        // 默认UI材质
        this->_createDefaultMaterial();
        // 默认UI网格
        this->_createDefaultMesh();
    }
    void UIMask::_createDefaultMaterial()
    {
        MaterialAsset *mtlAdd = dynamic_cast<MaterialAsset *>(assetManager->getAsset(AssetBuiltinMaterial::UIMaskAdd)); // AssetBuiltinMaterial::UIMaskAdd
        this->_addMaterialAsset = new MaterialAsset();
        this->_addMaterialAsset->create(mtlAdd);

        MaterialAsset *mtlSub = dynamic_cast<MaterialAsset *>(assetManager->getAsset(AssetBuiltinMaterial::UIMaskSub));
        this->_subMaterialAsset = new MaterialAsset();
        this->_subMaterialAsset->create(mtlSub);
    }

    void UIMask::_createDefaultMesh()
    {
        this->_maskMesh = dynamic_cast<MeshAsset *>(assetManager->getAsset(AssetBuiltinMesh::UI));
    }

    void UIMask::OnAwake()
    {
        UIRenderer::OnAwake();
    }
    void UIMask::OnEnable()
    {
        UIRenderer::OnEnable();
    }
    void UIMask::Update(float deltaTime)
    {
        UIRenderer::Update(deltaTime);
    }
    void UIMask::LateUpdate(float deltaTime)
    {
        UIRenderer::LateUpdate(deltaTime);
    }
    void UIMask::Render(Camera *camera)
    {
        if (!this->_canRenderer(camera))
        {
            return;
        }
        if (this->_addMaterialAsset == nullptr)
        {
            return;
        }
        if (this->_subMaterialAsset == nullptr)
        {
            return;
        }
        if (this->_maskMesh == nullptr)
        {
            return;
        }
        UIRenderer::Render(camera);
        // 后续添加节点层级alpha测试
        const Mat4 &matrix = this->_node2D->getUIWorldMatrix();
        Mat4::multiply(matrix, view->getFitMatrix(), this->_uiViewMatrix);
        // 更新世界矩阵
        this->_addMaterialAsset->setModelWorldMatrix(this->_uiViewMatrix.data());
        GfxManager::getInstance()->submitRenderObject(camera->getUuid(), this->_addMaterialAsset->getGfxMaterial(), this->_maskMesh->getGfxMesh(0));
        profiler->addObjectCount(1);
    }
    void UIMask::lateRender(Camera *camera)
    {
        if (!this->_canRenderer(camera))
        {
            return;
        }
        if (this->_addMaterialAsset == nullptr)
        {
            return;
        }
        if (this->_subMaterialAsset == nullptr)
        {
            return;
        }
        if (this->_maskMesh == nullptr)
        {
            return;
        }
        UIRenderer::Render(camera);
        // 后续添加节点层级alpha测试
        const Mat4 &matrix = this->_node2D->getUIWorldMatrix();
        Mat4::multiply(matrix, view->getFitMatrix(), this->_uiViewMatrix);
        // 更新世界矩阵
        this->_subMaterialAsset->setModelWorldMatrix(this->_uiViewMatrix.data());
        GfxManager::getInstance()->submitRenderObject(camera->getUuid(), this->_subMaterialAsset->getGfxMaterial(), this->_maskMesh->getGfxMesh(0));
    }
    void UIMask::OnDisable()
    {
        UIRenderer::OnDisable();
    }
    void UIMask::destroy()
    {
        UIRenderer::destroy();
    }
    UIMask::~UIMask()
    {
        if (this->_addMaterialAsset != nullptr)
		{
			this->_addMaterialAsset->destroy();
			delete this->_addMaterialAsset;
			this->_addMaterialAsset = nullptr;
		}
        if (this->_subMaterialAsset != nullptr)
		{
			this->_subMaterialAsset->destroy();
			delete this->_subMaterialAsset;
			this->_subMaterialAsset = nullptr;
		}
        this->_maskMesh=nullptr;
    }

} // namespace Boo
