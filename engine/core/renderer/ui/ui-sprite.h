#pragma once
#include <string>
#include "ui-renderer.h"
#include "../../math/color.h"
#include "../../component/component-register.h"
#include "../../component/component-property-register.h"

class Node;

class UISprite : public UIRenderer
{
public:
    REGISTER_PROPERTY_STRING(UISprite, _texture, "纹理");
    REGISTER_PROPERTY_STRING(UISprite, _material, "材质");
    
protected:
    /**
     * @brief 反序列化组件属性-配置
     * 反序列化成功
     */
    void _deserialized() override;
    /**
     * 更新渲染状态
     */
    void _updateRendererState() override;
    /**
     * 更新模型矩阵
     */
    void _updateModelMatrix() override;

public:
    UISprite(std::string name, Node *node, std::string uuid = "");

    void Awake() override;
    void Enable() override;

    /**
     * @brief 设置渲染器的颜色
     *
     * @param color
     */
    void setColor(Color &color);
    void setColor(std::string color);
    void setColor(float r, float g, float b, float a);
    /**
     * @brief 设置渲染器的透明度
     *
     * @param alpha
     */
    void setAlpha(float alpha);
    /**
     * @brief 设置渲染器的材质
     *
     * @param mtl
     */
    void setMaterialAsset(std::string mtl);
    void setMaterialAsset(MaterialAsset *mtl);
    /**
     * @brief 设置渲染器的纹理
     *
     * @param texture
     */
    void setTextureAsset(std::string texture);
    void setTextureAsset(TextureAsset *texture);

    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;
    void Render() override;
    void destroy() override;
    ~UISprite() override;
};
REGISTER_COMPONENT(UISprite, "UI Sprite Component")
