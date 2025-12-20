#pragma once
#include "node.h"
#include <vector>
#include <memory>
#include "node-3d.h"
#include "node-2d.h"

class Scene : public Node
{
private:
protected:

public:
    Scene(const std::string name, const std::string uuid = "");
    // void initRootNode();
    // Node3D *getRoot3D();
    // Node2D *getRoot2D();
    void setActive(bool active) override;
    /**
     * 添加组件
     */
    Component *addComponent(std::string name, std::string uuid = "") override;

    void update(float deltaTime) override;
    void lateUpdate(float deltaTime) override;
    void clearNodeFrameFlag() override;

    void destroy() override;
    ~Scene();
};