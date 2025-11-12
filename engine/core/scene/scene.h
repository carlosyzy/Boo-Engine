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
    Node3D *_root3D;
    Node2D *_root2D;

public:
    Scene(const std::string name, const std::string uuid = "");

    Node3D *getRoot3D() { return this->_root3D; }
    Node2D *getRoot2D() { return this->_root2D; }

    void update(float deltaTime) override;
    void lateUpdate(float deltaTime) override;
    void render();
    void clearNodeFrameFlag() override;

    Component *addComponent(std::string name, std::string uuid = "") override;
    Component *getComponent(std::string name) override;
    void destroy() override;
    ~Scene();
};