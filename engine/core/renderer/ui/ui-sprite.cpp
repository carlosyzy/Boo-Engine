#include "ui-sprite.h"
#include "../../scene/node.h"


UISprite::UISprite(Node *node, std::string uuid) : UIRenderer(node, uuid)
{
    this->_name = "UISprite";
}
void UISprite::Update(float deltaTime)
{
    UIRenderer::Update(deltaTime);
}
void UISprite::LateUpdate(float deltaTime) {
    UIRenderer::LateUpdate(deltaTime);
}
void UISprite::Render() {
    UIRenderer::Render();
}
void UISprite::destroy() {
    UIRenderer::destroy();
    std::cout << "UISprite::destroy" << std::endl;
}
UISprite::~UISprite()
{
   std::cout << "UISprite::~destructor" << std::endl;
}