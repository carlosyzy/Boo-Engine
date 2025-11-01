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
    ~Scene();
    Node3D *getRoot3D() { return this->_root3D; }
    Node2D *getRoot2D() { return this->_root2D; }   

    virtual void update(float deltaTime);
    virtual void lateUpdate(float deltaTime);
    virtual void render();
};