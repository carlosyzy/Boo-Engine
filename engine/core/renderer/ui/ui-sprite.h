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
    void update(float deltaTime) override;
    void lateUpdate(float deltaTime) override;
    void render() override;
    void destroy() override;
    ~UISprite() override;
};
