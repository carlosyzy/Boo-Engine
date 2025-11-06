#include "ui-sprite.h"
#include "../../scene/node.h"


UISprite::UISprite(Node *node, std::string uuid) : UIRenderer(node, uuid)
{
    this->_name = "UISprite";
}
void UISprite::update(float deltaTime)
{
    UIRenderer::update(deltaTime);
}
void UISprite::lateUpdate(float deltaTime) {
    UIRenderer::lateUpdate(deltaTime);
}
void UISprite::render() {
    UIRenderer::render();
}
void UISprite::destroy() {

}
UISprite::~UISprite()
{
}