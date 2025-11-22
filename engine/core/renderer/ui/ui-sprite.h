#pragma once
#include <string>
#include "ui-renderer.h"
class Node;

class UISprite : public UIRenderer
{
private:
    std::string _texture;

public:
    UISprite(Node *node, std::string uuid="");
    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;
    void Render() override;
    void destroy() override;
    ~UISprite() override;
};
