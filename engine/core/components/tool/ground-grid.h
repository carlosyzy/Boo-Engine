#pragma once
#include <string>
#include "core/component/component.h"
#include "core/component/component-register.h"

/**
 * @brief 地面网格组件
 *
 */
namespace Boo
{
    class Node;
    class Camera;
    class MaterialAsset;
    class TextureAsset;
    class Node3D;
    class MeshAsset;
    class MeshRenderer;
    class ShaderAsset;

    class GroundGrid : public Component
    {
    private:
        Node3D *_node;
        MeshAsset *_meshAsset;
        MaterialAsset *_materialAsset;
        MeshRenderer *_meshRenderer;
        ShaderAsset *_vertShaderAsset;
        ShaderAsset *_fragShaderAsset;

        void _createMesh();
        void _createMaterial();
        void _createShader();
        void _createMeshRenderer();

    public:
        GroundGrid(std::string name, Node *node, std::string uuid);
        void OnAwake() override;
        void OnEnable() override;
        void setProperty(json &data) override;
        void Update(float deltaTime) override;
        void LateUpdate(float deltaTime) override;
        void OnDisable() override;
        void OnDestroy() override {};
        void destroy() override;
        ~GroundGrid() override;
    };
    REGISTER_COMPONENT(GroundGrid, "Ground Grid Component")
}