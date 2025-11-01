#pragma once
#include "node.h"

class Node2D : public Node {
public:
    Node2D(const std::string name, const std::string uuid = "");
    ~Node2D();
    // 重写基类方法
    virtual void update(float deltaTime);
    virtual void lateUpdate(float deltaTime);
    virtual void render();
};