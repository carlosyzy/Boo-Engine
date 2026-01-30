#include "ui-sprite.h"
#include "../../boo.h"
#include "../../scene/node.h"
#include "../../scene/node-2d.h"
#include "../../assets/asset.h"
#include "../../assets/assets-manager.h"
#include "../../assets/texture-asset.h"
#include "../../assets/material-asset.h"
#include "../../renderer/camera.h"
#include "../../gfx/base/gfx-render-texture.h"

UISprite::UISprite(std::string name, Node *node, std::string uuid) : UIRenderer(name, node, uuid)
{
    // 纹理路径
    this->_texture = "";
    // 材质路径
    this->_material = "";

    this->_materialAsset = new MaterialAsset();
    // this->_materialAsset->createUITest();
}
/**
 * @brief 反序列化组件属性-配置
 * 反序列化成功
 */
void UISprite::_deserialized()
{
    UIRenderer::_deserialized();
}

void UISprite::Awake()
{
    UIRenderer::Awake();
}

void UISprite::Enable()
{
    UIRenderer::Enable();
}

void UISprite::setColor(Color &color)
{
    if (color.getR() == this->_color.getR() &&
        color.getG() == this->_color.getG() &&
        color.getB() == this->_color.getB() &&
        color.getA() == this->_color.getA())
    {
        return;
    }
    this->_setColor(color.getR(), color.getG(), color.getB(), color.getA());
}
void UISprite::setColor(std::string color)
{
    if (color == this->_color.hexString())
    {
        return;
    }
    Color c(color);
    this->_setColor(c.getR(), c.getG(), c.getB(), c.getA());
}
void UISprite::setColor(float r, float g, float b, float a)
{
    if (r == this->_color.getR() &&
        g == this->_color.getG() &&
        b == this->_color.getB() &&
        a == this->_color.getA())
    {
        return;
    }
    this->_setColor(r, g, b, a);
}

void UISprite::setAlpha(float alpha)
{
    if (alpha == this->_color.getA())
        return;
    Color c(this->_color);
    c.setA(alpha);
    this->_setColor(c.getR(), c.getG(), c.getB(), c.getA());
}
void UISprite::setMaterial(std::string material)
{
    // MaterialAsset *mtl = dynamic_cast<MaterialAsset *>(Boo::game->assetsManager()->getAssetByPath(material));
    // if (mtl == nullptr)
    // {
    //     std::cout << "UISprite::setMaterial: material " << material << " not found" << std::endl;
    //     return;
    // }
    // this->_setMaterial(mtl);
}
void UISprite::setMaterial(MaterialAsset *material)
{
    if (material == nullptr)
    {
        std::cout << "UISprite::setMaterial: material " << material << " not found" << std::endl;
        return;
    }
    this->_setMaterial(material);
}

void UISprite::setTexture(std::string path)
{
}
void UISprite::setTexture(TextureAsset *texture)
{
    if (texture == nullptr)
    {
        std::cout << "UISprite::setTexture: texture " << texture << " not found" << std::endl;
        return;
    }
    this->_setTexture(texture);
}
void UISprite::setRenderTexture(GfxRenderTexture *renderTexture)
{
    this->_renderTexture = renderTexture;
    if (this->_renderTexture != nullptr)
    {
        // std::cout << "UISprite::setRenderTexture: renderTexture: " << this->_renderTexture->getColorTextureUuid() << std::endl;
        this->_materialAsset->setTexture(0, this->_renderTexture->getColorTextureUuid());
    }
}

void UISprite::Update(float deltaTime)
{
    UIRenderer::Update(deltaTime);
}
void UISprite::LateUpdate(float deltaTime)
{
    UIRenderer::LateUpdate(deltaTime);
}
void UISprite::Render(Camera *camera)
{
    UIRenderer::Render(camera);
}
void UISprite::Disable()
{
    UIRenderer::Disable();
}

void UISprite::destroy()
{
    UIRenderer::destroy();
    // std::cout << "UISprite::destroy" << std::endl;
}
UISprite::~UISprite()
{
    // std::cout << "UISprite::~destructor" << std::endl;
}