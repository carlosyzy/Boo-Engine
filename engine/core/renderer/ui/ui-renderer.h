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
#include "core/math/mat4.h"
namespace Boo
{

    class Node;
    class Camera;
    class MaterialAsset;
    class TextureAsset;
    class MeshAsset;
    class Node2D;

    class UIRenderer : public Component
    {
    private:
    protected:
        Node2D *_node2D;
        Mat4 _uiViewMatrix;
        /**
         * @brief 检查渲染器是否可以渲染
         *
         */
        bool _canRenderer(Camera *camera);

    public:
        UIRenderer(std::string name, Node *node, std::string uuid = "");

        void OnAwake() override;
        void OnEnable() override;
        void setProperty(json &data) override;
        void Update(float deltaTime) override;
        void LateUpdate(float deltaTime) override;
        /**
         * @brief 组件渲染函数
         * 当前节点组件以及子节点组件的lateUpdate执行结束,render渲染函数开始
         */
        virtual void Render(Camera *camera);
        void OnDisable() override;
        void OnDestroy() override {};
        void destroy() override;

        virtual ~UIRenderer() override;
    };
} // namespace Boo
