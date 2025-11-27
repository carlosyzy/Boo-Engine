#pragma once
#include <string>
#include <iostream>
#include "ui-renderer.h"
#include "../../math/color.h"
#include "../../component/component-property-register.h"
#include "../../component/component-register.h"
#include "../../scene/node.h"
#include "../../font/freetype-mgr.h"
class TextureAsset;
class UIText : public UIRenderer
{

public:
    REGISTER_PROPERTY_STRING(UIText, _text, "文本");
    REGISTER_PROPERTY_STRING(UIText, _material, "材质");
    REGISTER_PROPERTY_INT(UIText, _fontSize, "字体大小");
    REGISTER_PROPERTY_INT(UIText, _lineHeight, "行高");

private:
    TextTexture _fontTexture;
    TextureAsset *_texture = nullptr;

    void _updateNodeSize();
    void _clearTexture();

protected:
    /**
     * @brief 反序列化组件属性-配置
     * 反序列化成功后，设置文本和材质
     */
    void _deserialized() override;
    /**
     * 更新渲染状态
     */
    virtual void _updateRendererState();
    /**
     * 更新模型矩阵
     */
    virtual void _updateModelMatrix();


public:
    UIText(std::string name, Node *node, std::string uuid = "");
    void setText(std::string text);
    void setSize(int fontSize);
    void setLineHeight(int lineHeight);

    void setColor(Color &color);
    void setColor(std::string color);
    void setColor(float r, float g, float b, float a);
    void setAlpha(float alpha);
    void setMaterialAsset(std::string mtl);
    void setMaterialAsset(MaterialAsset *mtl);

    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;
    void Render() override;
    void destroy() override;
    ~UIText() override;
};
// 注册宏调用
REGISTER_COMPONENT(UIText, "UI文本组件")