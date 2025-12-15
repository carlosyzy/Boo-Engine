#include "ui-sprite.h"
#include "../../boo.h"
#include "../../scene/node.h"
#include "../../scene/node-2d.h"
#include "../../assets/asset.h"
#include "../../assets/assets-manager.h"
#include "../../assets/texture-asset.h"
#include "../../assets/material-asset.h"
#include "../../renderer/camera.h"

UISprite::UISprite(std::string name, Node *node, std::string uuid) : UIRenderer(name, node, uuid)
{
    // 纹理路径
    this->_texture = "";
    // 材质路径
    this->_material = "";
}
/**
 * @brief 反序列化组件属性-配置
 * 反序列化成功
 */
void UISprite::_deserialized()
{
    UIRenderer::_deserialized();
    // this->setTextureAsset(this->_texture);
    // this->setMaterialAsset(this->_material);
}

void UISprite::Awake()
{
    UIRenderer::Awake();
}

void UISprite::Enable()
{
    UIRenderer::Enable();
    Boo::game->extractUIRenderer(this);
}

void UISprite::setColor(Color &color)
{
   if(color.getR() == this->_color.getR() &&
      color.getG() == this->_color.getG() &&
      color.getB() == this->_color.getB() &&
      color.getA() == this->_color.getA()){
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

void UISprite::setTextureAsset(std::string path)
{
    // TextureAsset *tex = dynamic_cast<TextureAsset *>(Boo::game->assetsManager()->getAssetByPath(texture));
    // if (tex == nullptr)
    // {
    //     std::cout << "UISprite::setTexture: texture " << texture << " not found" << std::endl;
    //     return;
    // }
    // this->_setTexture(tex);
}
void UISprite::setTextureAsset(TextureAsset *texture)
{
    if (texture == nullptr)
    {
        std::cout << "UISprite::setTexture: texture " << texture << " not found" << std::endl;
        return;
    }
    this->_setTexture(texture);
}


void UISprite::setMaterialAsset(std::string mtl)
{
    // MaterialAsset *mtlAsset = dynamic_cast<MaterialAsset *>(Boo::game->assetsManager()->get(mtl));
    // if (mtlAsset == nullptr)
    // {
    //     std::cout << "UISprite::setMaterial: material " << mtl << " not found" << std::endl;
    //     return;
    // }
    // this->_setMaterial(mtlAsset);
}
void UISprite::setMaterialAsset(MaterialAsset *mtl)
{
    // if (mtl == nullptr)
    // {
    //     std::cout << "UISprite::setMaterial: material " << mtl << " not found" << std::endl;
    //     return;
    // }
    // this->_setMaterial(mtl);
}


// void UISprite::_updateRendererState()
// {
//     UIRenderer::_updateRendererState();
// }
// void UISprite::_updateModelMatrix()
// {
//     UIRenderer::_updateModelMatrix();
// }

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
    Boo::game->removeUIRenderer(this);
}

void UISprite::destroy()
{
    UIRenderer::destroy();
    std::cout << "UISprite::destroy" << std::endl;
}
UISprite::~UISprite()
{
    std::cout << "UISprite::~destructor" << std::endl;
}