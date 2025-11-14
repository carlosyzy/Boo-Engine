#pragma once
#include <string>
#include "ui-renderer.h"
#include "../../scene/node.h"
#include "../../font/freetype-mgr.h"
class Texture;
class UIText : public UIRenderer
{
private:
    std::string _text;
    TextTexture _fontTexture;
    int _fontSize = 30;
    int _lineHeight = 30;
    Texture *_texture = nullptr;
    void _updateNodeSize();
    void _clearTexture();
public:
    UIText(Node *node, std::string uuid = "");
    void setText(std::string text);
    void setSize(int fontSize);
    void setLineHeight(int lineHeight);
    void update(float deltaTime) override;
    void lateUpdate(float deltaTime) override;
    void render() override;
    void destroy() override;
     ~UIText() override;
};