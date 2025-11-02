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
void UISprite::destroy() {

}
UISprite::~UISprite()
{
}