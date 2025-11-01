#pragma once
#include "node.h"

class Node3D : public Node {
public:
    Node3D(const std::string name, const std::string uuid = "");
    ~Node3D();
    // 重写基类方法
    virtual void update(float deltaTime);
    virtual void lateUpdate(float deltaTime);
    virtual void render();
};