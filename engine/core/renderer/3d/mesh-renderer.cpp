#include "mesh-renderer.h"
#include "boo.h"
#include "log.h"
#include "core/asset/mesh-asset.h"
#include "core/asset/material-asset.h"
#include "core/asset/texture-asset.h"
#include "core/scene/node-3d.h"
#include "core/gfx/gfx-manager.h"
#include "core/renderer/camera.h"

namespace Boo
{
    MeshRenderer::MeshRenderer(std::string name, Node *node, std::string uuid) : Component(name, node, uuid),
                                                                                 _meshAsset(nullptr),
                                                                                 _materials({})
    {
        this->_layer = EComponentLayer::Layer3D;
        this->_node3D = dynamic_cast<Node3D *>(node);
        this->_meshAsset = nullptr;
        this->_materials.clear();
    }

    void MeshRenderer::OnAwake()
    {
        Component::OnAwake();
    }
    void MeshRenderer::setProperty(json &data)
    {
        Component::setProperty(data);
        // set mesh asset
        if (data.contains("mesh") && data["mesh"].is_string())
        {
            this->setMesh(data["mesh"].get<std::string>());
        }
        // set material asset
        if (data.contains("material") && data["material"].is_array())
        {
            for (int i = 0; i < data["material"].size(); i++)
            {
                this->setMaterial(i, data["material"][i].get<std::string>());
            }
        }
    }

    void MeshRenderer::OnEnable()
    {
        Component::OnEnable();
    }
    void MeshRenderer::setMesh(std::string meshUuid)
    {
        Asset *asset = assetManager->getAsset(meshUuid);
        MeshAsset *meshAsset = dynamic_cast<MeshAsset *>(asset);
        if (meshAsset == nullptr)
        {
            LOGW("MeshRenderer::setMesh: meshAsset is nullptr");
            return;
        }
        this->setMesh(meshAsset);
    }
    void MeshRenderer::setMesh(MeshAsset *meshAsset)
    {
        if (meshAsset == nullptr)
        {
            LOGW("MeshRenderer::setMeshAsset: meshAsset is nullptr");
            return;
        }
        this->_meshAsset = meshAsset;
        this->_clearMaterials();
        this->_resetMaterials();
    }
    void MeshRenderer::setMaterial(std::string materialName)
    {
        Asset *asset = assetManager->getAsset(materialName, true);
        MaterialAsset *materialAsset = dynamic_cast<MaterialAsset *>(asset);
        if (materialAsset == nullptr)
        {
            LOGW("MeshRenderer::setMaterialAsset: materialAsset is nullptr");
            return;
        }
        this->setMaterial(0, materialAsset);
    }
    void MeshRenderer::setMaterial(MaterialAsset *materialAsset)
    {
        this->setMaterial(0, materialAsset);
    }
    void MeshRenderer::setMaterial(int index, std::string materialName)
    {
        Asset *asset = assetManager->getAsset(materialName, true);
        MaterialAsset *materialAsset = dynamic_cast<MaterialAsset *>(asset);
        if (materialAsset == nullptr)
        {
            LOGW("MeshRenderer::setMaterialAsset: materialAsset is nullptr");
            return;
        }
        this->setMaterial(index, materialAsset);
    }
    void MeshRenderer::setMaterial(int index, MaterialAsset *materialAsset)
    {
        if (this->_meshAsset == nullptr)
        {
            LOGW("MeshRenderer::setMaterialAsset: meshAsset is nullptr");
            return;
        }
        if (index < 0 || index >= this->_materials.size())
        {
            LOGW("MeshRenderer::setMaterialAsset: index out of range");
            return;
        }
        std::cout << "MeshRenderer::setMaterialAsset: materialAsset created" << std::endl;
        this->_materials[index]->create(materialAsset);
    }
    void MeshRenderer::setTexture(std::string path)
    {
        Asset *asset = assetManager->getAsset(path, true);
        TextureAsset *texture = dynamic_cast<TextureAsset *>(asset);
        if (texture == nullptr)
        {
            LOGW("MeshRenderer::setTextureAsset: texture is nullptr");
            return;
        }
        this->setTexture(texture);
    }
    void MeshRenderer::setTexture(TextureAsset *texture)
    {
        if (texture == nullptr)
        {
            LOGW("MeshRenderer::setTextureAsset: texture is nullptr");
            return;
        }
        this->_materials[0]->setTexture(texture);
    }

    std::vector<MaterialAsset *> MeshRenderer::getMaterials()
    {
        return this->_materials;
    }
    MaterialAsset *MeshRenderer::getMaterial()
    {
        if (this->_materials.empty())
        {
            LOGW("MeshRenderer::getMaterial: materials is empty");
            return nullptr;
        }
        return this->_materials[0];
    }
    MaterialAsset *MeshRenderer::getMaterial(int index)
    {
        if (index < 0 || index >= this->_materials.size())
        {
            LOGW("MeshRenderer::getMaterial: index out of range");
            return nullptr;
        }
        return this->_materials[index];
    }
    void MeshRenderer::Update(float deltaTime)
    {
        Component::Update(deltaTime);
    }
    void MeshRenderer::LateUpdate(float deltaTime)
    {
        Component::LateUpdate(deltaTime);
    }
    /**
     * @brief 组件渲染函数
     * 当前节点组件以及子节点组件的lateUpdate执行结束,render渲染函数开始
     */
    void MeshRenderer::Render(Camera *camera)
    {
        if (camera == nullptr)
        {
            return;
        }
        if (this->_meshAsset == nullptr)
        {
            LOGW("MeshRenderer::Render: meshAsset is nullptr");
            return;
        }
        if (this->_materials.empty())
        {
            LOGW("MeshRenderer::Render: materials is empty");
            return;
        }

        const Mat4 &matrix = this->_node3D->getWorldMatrix();
        const Mat4 &worldIT = this->_node3D->getWorldMatrixIT();
        size_t subMeshCount = this->_meshAsset->getSubMeshCount();
        for (size_t i = 0; i < subMeshCount; i++)
        {
            GfxMesh *gfxMesh = this->_meshAsset->getGfxMesh(i);
            if (gfxMesh == nullptr)
            {
                continue;
            }
            if (i >= this->_materials.size())
            {
                continue;
            }
            if (this->_materials[i] == nullptr)
            {
                continue;
            }
            this->_materials[i]->setModelWorldMatrix(matrix.data());
            this->_materials[i]->setModelWorldMatrixIT(worldIT.data());
            GfxMaterial *gfxMaterial = this->_materials[i]->getGfxMaterial();
            GfxManager::getInstance()->submitRenderObject(camera->getUuid(), gfxMaterial, gfxMesh);
        }
        // 增加渲染物体数量
        profiler->addObjectCount(1);
    }
    void MeshRenderer::_resetMaterials()
    {
        if (this->_meshAsset == nullptr)
        {
            LOGW("MeshRenderer::_resetMaterials: meshAsset is nullptr");
            return;
        }
        MaterialAsset *defaultMaterial = dynamic_cast<MaterialAsset *>(assetManager->getAsset(AssetBuiltinMaterial::Unlit));
        if (defaultMaterial == nullptr)
        {
            LOGW("MeshRenderer::_resetMaterials: default materialAsset is nullptr");
            return;
        }
        for (size_t i = 0; i < this->_meshAsset->getSubMeshCount(); i++)
        {
            MaterialAsset *material = new MaterialAsset();
            material->create(defaultMaterial);
            this->_materials.push_back(material);
        }
    }
    void MeshRenderer::_clearMaterials()
    {
        for (size_t i = 0; i < this->_materials.size(); i++)
        {
            this->_materials[i]->destroy();
            delete this->_materials[i];
            this->_materials[i] = nullptr;
        }
        this->_materials.clear();
    }

    void MeshRenderer::OnDisable()
    {
        Component::OnDisable();
    }
    void MeshRenderer::destroy()
    {
        Component::destroy();
    }
    MeshRenderer::~MeshRenderer()
    {
        for (auto mtl : this->_materials)
        {
            mtl->destroy();
            delete mtl;
            mtl = nullptr;
        }
        this->_materials.clear();
        this->_node3D = nullptr;
        this->_meshAsset = nullptr;
    }
} // namespace Boo
