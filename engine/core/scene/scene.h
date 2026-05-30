#pragma once
#include "node.h"
#include <vector>
#include <memory>
#include "core/scene/node-2d.h"
#include "core/scene/node-3d.h"
namespace Boo
{
    class Scene : public Node
    {
    public:
        Scene(const std::string name, const std::string uuid = "");

    private:
        Node3D *_root3D;
        Node2D *_root2D;

    public:
        void createRoot2D();
        void createRoot3D();
        Node3D *getRoot3D();
        Node2D *getRoot2D();

    protected:
        void _updateWorldTransform() override;

    public:
        void setPosition(float x, float y, float z) override;
        void setWorldPosition(float x, float y, float z) override;
        void setScale(float x, float y, float z) override;
        void setWorldScale(float x, float y, float z) override;
        void setEulerAngles(float x, float y, float z) override;
        void setRotation(float x, float y, float z, float w) override;
        void setWorldRotation(float x, float y, float z, float w) override;
        void setActive(bool active) override;
        void launch();
        Component *addComponent(std::string name, std::string uuid = "") override;
        void update(float deltaTime) override;
        void lateUpdate(float deltaTime) override;
        void clearNodeFrameFlag() override;
        void destroy() override;

        ~Scene();
    };

} // namespace Boo