#pragma once
#include "node.h"

class Node3D : public Node {
public:
    Node3D(const std::string name, const std::string uuid = "");
    // 重写基类方法
    void update(float deltaTime) override;
    void lateUpdate(float deltaTime) override;
    void render();
    void clearNodeFrameFlag() override;
    void destroy() override;
    ~Node3D() override;
};