#pragma once
#include "node-base.h"

class Node3D : public NodeBase {
public:
    Node3D(const std::string& name, const std::string& uuid = "");
    ~Node3D();
    // 重写基类方法
    virtual void update() override;
    virtual void render() override;
};