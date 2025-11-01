#pragma once
#include "node.h"
#include <vector>
#include <memory>

class Scene : public Node {
protected:
   

public:
    Scene(const std::string name, const std::string uuid = "");
    ~Scene();

    virtual void update(float deltaTime);
    virtual void lateUpdate(float deltaTime);
    virtual void render();
};