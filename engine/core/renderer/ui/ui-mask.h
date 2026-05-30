#pragma once
#include <string>
#include <iostream>
#include "ui-renderer.h"
#include "core/component/component-register.h"
namespace Boo
{
    class Node;
    class Camera;
    class MaterialAsset;
    class TextureAsset;
    class MeshAsset;

    /**
     * mask 组件继承与ui-renderer组件
     * 后续优化
     */

    class UIMask : public UIRenderer
    {
    private:
        MaterialAsset *_addMaterialAsset;
        MaterialAsset *_subMaterialAsset;
        MeshAsset *_maskMesh;
        void _createDefaultMaterial();
        void _createDefaultMesh();

    public:
        UIMask(std::string name, Node *node, std::string uuid = "");
        void OnAwake() override;
        void OnEnable() override;

        void Update(float deltaTime) override;
        void LateUpdate(float deltaTime) override;
        void Render(Camera *camera) override;
        void lateRender(Camera *camera);
        void OnDisable() override;
        void OnDestroy() override {};
        void destroy() override;
        ~UIMask() override;
    };
    REGISTER_COMPONENT(UIMask, "UI Mask Component")
} // namespace Boo
