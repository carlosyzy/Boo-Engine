#pragma once
#include "node-base.h"
#include <vector>
#include <memory>

class Scene : public NodeBase {
protected:
   

public:
    Scene(const std::string& name, const std::string& uuid = "");
    ~Scene();

    // 场景操作
    virtual void update();
    virtual void render();
};