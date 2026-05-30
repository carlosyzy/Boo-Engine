#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <array>
#include <cstdint>

#include "core/component/component.h"
#include "core/component/component-register.h"

namespace Boo
{
    class Node;
    class Camera;
    class MaterialAsset;
    class TextureAsset;
    class Node3D;
    class MeshAsset;

    class MeshRenderer : public Component
    {
    private:
    protected:
        Node3D *_node3D;
        MeshAsset *_meshAsset;
        std::vector<MaterialAsset *> _materials;

        void _clearMaterials();
        void _resetMaterials();

    public:
        MeshRenderer(std::string name, Node *node, std::string uuid = "");
        void OnAwake() override;
        void OnEnable() override;
        void setProperty(json &data) override;
        std::vector<MaterialAsset *> getMaterials();
        MaterialAsset *getMaterial();
        MaterialAsset *getMaterial(int index);

        void setMesh(std::string meshUuid);
        /**
         * @brief 设置网格资产
         * @param meshAsset 网格资产
         */
        void setMesh(MeshAsset *meshAsset);
        /**
         * @brief 设置材质资产
         * @param materialName 材质名称
         */
        void setMaterial(std::string materialName);
        void setMaterial(MaterialAsset *materialAsset);
        void setMaterial(int index, std::string materialName);
        void setMaterial(int index, MaterialAsset *materialAsset);

        void setTexture(std::string path);
        void setTexture(TextureAsset *texture);

        void Update(float deltaTime) override;
        void LateUpdate(float deltaTime) override;
        /**
         * @brief 组件渲染函数
         * 当前节点组件以及子节点组件的lateUpdate执行结束,render渲染函数开始
         */
        void Render(Camera *camera);
        void OnDisable() override;
        void OnDestroy() override {};
        void destroy() override;
        ~MeshRenderer() override;
    };
    REGISTER_COMPONENT(MeshRenderer, "Mesh Renderer Component")
} // namespace Boo