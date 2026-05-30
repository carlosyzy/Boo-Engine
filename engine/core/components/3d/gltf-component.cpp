#include "gltf-component.h"
#include "boo.h"
#include "log.h"
#include "core/scene/node-3d.h"
#include "core/renderer/3d/mesh-renderer.h"
#include "core/asset/gltf-asset.h"
#include "core/asset/mesh-asset.h"
#include "core/asset/material-asset.h"
#include "core/asset/texture-asset.h"

namespace Boo
{
    GLTFComponent::GLTFComponent(std::string name, Node *node, std::string uuid)
        : Component(name, node, uuid)
    {
        this->_layer = EComponentLayer::Layer3D;
        this->_node3D = dynamic_cast<Node3D *>(node);
        this->_gltfAsset = nullptr;
    }
    void GLTFComponent::  OnAwake()
    {
    }
    void GLTFComponent::  OnEnable ()
    {
    }
    void GLTFComponent::setProperty(json &data)
    {
        Component::setProperty(data);
        if (data.contains("gltf")&&data["gltf"].is_string())
        {
           this->setGLTFAsset(data["gltf"].get<std::string>());
        }
    }
    void GLTFComponent::setGLTFAsset(std::string gltfAssetPath)
    {
        GLTFAsset *asset = dynamic_cast<GLTFAsset *>(assetManager->getAsset(gltfAssetPath,true));
        if (asset == nullptr)
        {
            LOGI("GLTFComponent::setGLTFAsset, asset: %s not found", gltfAssetPath.c_str());
            return;
        }
        this->setGLTFAsset(asset);
    }
    void GLTFComponent::setGLTFAsset(GLTFAsset *gltfAsset)
    {
        if (this->_gltfAsset != nullptr && this->_gltfAsset->getUuid() == gltfAsset->getUuid())
        {
            LOGW("GLTFComponent::setGLTFAsset, gltfAsset: %s already set", gltfAsset->getUuid().c_str());
            return;
        }
        this->_gltfAsset = gltfAsset;
        this->_parseGLTFAsset(gltfAsset);
    }
    void GLTFComponent::_parseGLTFAsset(GLTFAsset *_gltfAsset)
    {
        if (_gltfAsset == nullptr)
        {
            LOGE("GLTFComponent::_parseGLTFAsset, gltfAsset: nullptr");
            return;
        }
        // const GLTFNode &root = this->_gltfAsset->getRoot();
        // this->_parseGLTFNode(this->_node3D, this->_gltfAsset->getRoot());
    }
    void GLTFComponent::_parseGLTFNode(Node3D *parent, const GLTFNode &gltfNode)
    {
        if (parent == nullptr)
        {
            LOGE("GLTFComponent::_parseGLTFNode, parent: nullptr");
            return;
        }
        if (gltfNode.children.empty())
        {
            // 无子节点，直接返回
            return;
        }
        for (const auto &child : gltfNode.children)
        {
            Node3D *node3D = new Node3D(child.name);
            parent->addChild(node3D);
            node3D->setMatrix(child.local);
            std::string meshUuid = child.meshUuid;
            if (!meshUuid.empty())
            {
                this->_parseGLTFMeshRenderer(node3D, meshUuid);
            }
            this->_parseGLTFNode(node3D, child);
        }
    }
    void GLTFComponent::_parseGLTFMeshRenderer(Node3D *node3D, const std::string &meshUuid)
    {
        if (node3D == nullptr)
        {
            LOGE("GLTFComponent::_parseGLTFMeshRenderer, node3D: nullptr");
            return;
        }
        if (meshUuid.empty())
        {
            LOGE("GLTFComponent::_parseGLTFMeshRenderer, meshUuid: empty");
            return;
        }
        MeshRenderer *meshRenderer = dynamic_cast<MeshRenderer *>(node3D->addComponent("MeshRenderer"));
        if (meshRenderer == nullptr)
        {
            LOGE("GLTFComponent::_parseGLTFMeshRenderer, meshRenderer: nullptr");
            return;
        }

        MeshAsset *meshAsset = dynamic_cast<MeshAsset *>(assetManager->getAsset(meshUuid,true));
        if (meshAsset == nullptr)
        {
            LOGE("GLTFComponent::_parseGLTFMeshRenderer, meshAsset: nullptr");
            return;
        }
        meshRenderer->setMesh(meshAsset);
    }
    void GLTFComponent::setMaterial(std::string meshNodePath, int meshIndex, std::string materialPath)
    {
        MaterialAsset *material = dynamic_cast<MaterialAsset *>(assetManager->getAsset(materialPath,true));
        if (material == nullptr)
        {
            LOGE("GLTFComponent::setMaterial, material: %s not found", materialPath.c_str());
            return;
        }
        this->setMaterial(meshNodePath, meshIndex, material);
    }
    void GLTFComponent::setMaterial(std::string meshNodePath, int meshIndex, MaterialAsset *material)
    {
        MeshRenderer *meshRenderer = this->_getMeshRenderer(meshNodePath);
        if (meshRenderer == nullptr)
            return;
        meshRenderer->setMaterial(meshIndex, material);
    }
    MeshRenderer *GLTFComponent::_getMeshRenderer(const std::string &meshNodePath)
    {
        if (this->_node3D == nullptr)
        {
            LOGE("GLTFComponent::_getMeshRenderer, _node3D: nullptr");
            return nullptr;
        }
        Node *current = this->_node3D;
        std::stringstream ss(meshNodePath);
        std::string segment;
        while (std::getline(ss, segment, '/'))
        {
            if (segment.empty())
                continue;
            current = current->getChildByName(segment);
            if (current == nullptr)
            {
                LOGE("GLTFComponent::_getMeshRenderer, node: %s not found in path: %s", segment.c_str(), meshNodePath.c_str());
                return nullptr;
            }
        }
        MeshRenderer *meshRenderer = dynamic_cast<MeshRenderer *>(current->getComponent("MeshRenderer"));
        if (meshRenderer == nullptr)
        {
            LOGE("GLTFComponent::_getMeshRenderer, MeshRenderer not found on node: %s", meshNodePath.c_str());
        }
        return meshRenderer;
    }
    // --- setTexture ---
    void GLTFComponent::setTexture(std::string meshNodePath, std::string texturePath)
    {
        this->setTexture(meshNodePath, 0, texturePath);
    }
    void GLTFComponent::setTexture(std::string meshNodePath, TextureAsset *texture)
    {
        this->setTexture(meshNodePath, 0, texture);
    }
    void GLTFComponent::setTexture(std::string meshNodePath, std::string key, std::string texturePath)
    {
        this->setTexture(meshNodePath, 0, key, texturePath);
    }
    void GLTFComponent::setTexture(std::string meshNodePath, std::string key, TextureAsset *texture)
    {
        this->setTexture(meshNodePath, 0, key, texture);
    }
    void GLTFComponent::setTexture(std::string meshNodePath, int mtlIndex, std::string texturePath)
    {
        TextureAsset *texture = dynamic_cast<TextureAsset *>(assetManager->getAsset(texturePath,true));
        if (texture == nullptr)
        {
            LOGE("GLTFComponent::setTexture, texture: %s not found", texturePath.c_str());
            return;
        }
        this->setTexture(meshNodePath, mtlIndex, texture);
    }
    void GLTFComponent::setTexture(std::string meshNodePath, int mtlIndex, TextureAsset *texture)
    {
        MeshRenderer *meshRenderer = this->_getMeshRenderer(meshNodePath);
        if (meshRenderer == nullptr)
            return;
        std::vector<MaterialAsset *> materials = meshRenderer->getMaterials();
        if (mtlIndex < 0 || mtlIndex >= (int)materials.size())
        {
            LOGE("GLTFComponent::setTexture, mtlIndex: %d out of range", mtlIndex);
            return;
        }
        materials[mtlIndex]->setTexture(texture);
    }
    void GLTFComponent::setTexture(std::string meshNodePath, int mtlIndex, std::string key, std::string texturePath)
    {
        TextureAsset *texture = dynamic_cast<TextureAsset *>(assetManager->getAsset(texturePath,true));
        if (texture == nullptr)
        {
            LOGE("GLTFComponent::setTexture, texture: %s not found", texturePath.c_str());
            return;
        }
        this->setTexture(meshNodePath, mtlIndex, key, texture);
    }
    void GLTFComponent::setTexture(std::string meshNodePath, int mtlIndex, std::string key, TextureAsset *texture)
    {
        MeshRenderer *meshRenderer = this->_getMeshRenderer(meshNodePath);
        if (meshRenderer == nullptr)
            return;
        std::vector<MaterialAsset *> materials = meshRenderer->getMaterials();
        if (mtlIndex < 0 || mtlIndex >= (int)materials.size())
        {
            LOGE("GLTFComponent::setTexture, mtlIndex: %d out of range", mtlIndex);
            return;
        }
        materials[mtlIndex]->setTexture(key, texture);
    }
    void GLTFComponent::  OnDisable()
    {
    }
    void GLTFComponent::Update(float deltaTime)
    {
    }
    void GLTFComponent::LateUpdate(float deltaTime)
    {
    }
    void GLTFComponent::destroy()
    {
    }

    GLTFComponent::~GLTFComponent()
    {
    }
} // namespace Boo
