#pragma once
#include "node.h"
#include <vector>
#include <memory>
class Node2D;
class Node3D;

class Scene : public Node
{
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

    /**
     * 添加组件
     */
    Component* addComponent(std::string name, std::string uuid = "") override;
    /*
     * 获取组件
     */
    Component* getComponent(std::string name) override;
    void destroy() override;
    ~Scene();
};