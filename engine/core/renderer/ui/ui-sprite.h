#pragma once
#include <string>
#include "ui-renderer.h"
#include "../../component/component-register.h"
#include "../../property/property-reflect-register.h"

class Node;

class UISprite : public UIRenderer
{
private:
    std::string _texture;

public:
    UISprite(std::string name, Node *node, std::string uuid="");
    REGISTER_PROPERTY_STRING(UISprite, spriteTexture, "精灵纹理");
    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;
    void Render() override;
    void destroy() override;
    ~UISprite() override;
};
REGISTER_COMPONENT(UISprite, "UISprite", "UI精灵组件")
