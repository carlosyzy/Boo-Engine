#pragma once
#include <string>
#include "../component.h"
#include "../../scene/node.h"
class Texture;
class UIMask : public Component
{
private:
    
public:
    UIMask(Node *node, std::string uuid = "");
    void update(float deltaTime) override;
    void lateUpdate(float deltaTime) override;
    void render() override;
    void destroy() override;
     ~UIMask() override;
};