#include "ui-text.h"
#include "../../../boo.h"
#include "../../scene/node.h"
#include "../../scene/node-2d.h"
#include "../../assets/texture.h"

UIText::UIText(Node *node, std::string uuid) : UIRenderer(node, uuid)
{
    this->_name = "UIText";
}

void UIText::setText(std::string text)
{
    this->_clearTexture();
    this->_text = text;
    Boo::game->fontMgr()->crateFont(this->_fontTexture, this->_text, this->_fontSize, this->_lineHeight);
    this->_texture = new Texture(this->_fontTexture.uuid, this->_fontTexture.width, this->_fontTexture.height, this->_fontTexture.channels, this->_fontTexture.datas);
    this->setTexture(this->_texture);
    this->_updateNodeSize();
}
void UIText::setSize(int fontSize)
{
    this->_fontSize = fontSize;
}
void UIText::setLineHeight(int lineHeight)
{
    this->_lineHeight = lineHeight;
}
void UIText::Update(float deltaTime)
{
    UIRenderer::Update(deltaTime);
}
void UIText::LateUpdate(float deltaTime)
{
    UIRenderer::LateUpdate(deltaTime);
}
void UIText::Render()
{
    UIRenderer::Render();
}
void UIText::_updateNodeSize()
{
    Node2D *node2d = dynamic_cast<Node2D *>(this->_node);
    if (node2d == nullptr)
    {
        return;
    }
    const Size &size = node2d->getSize();
    float width = this->_fontTexture.width;
    float height = this->_fontTexture.height;
    float scale = size.getHeight() / height;
    node2d->setSize(width * scale, height * scale);
}
void UIText::_clearTexture()
{
    if (this->_texture != nullptr)
    {
        this->_texture->destroy();
        this->_texture = nullptr;
    }
}

void UIText::destroy()
{
    UIRenderer::destroy();
}
UIText::~UIText()
{
}
