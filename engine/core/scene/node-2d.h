#pragma once
#include "node-base.h"

class Node2D : public NodeBase {
public:
    Node2D(const std::string& name, const std::string& uuid = "");
    ~Node2D();
    // 重写基类方法
    void update() override;
    void render() override;
};