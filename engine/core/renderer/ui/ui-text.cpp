#include "ui-text.h"
#include "../../assets/asset.h"
#include "../../assets/assets-manager.h"
#include "../../assets/material-asset.h"
#include "../../assets/texture-asset.h"
#include "../../boo.h"
#include "../../scene/node-2d.h"
#include "../../scene/node.h"

UIText::UIText(std::string name, Node *node, std::string uuid)
    : UIRenderer(name, node, uuid) {
  this->_positions = {
      -0.5f, 0.5f,  0.0f, /** @brief 左上 */
      -0.5f, -0.5f, 0.0f, /** @brief 坐下 */
      0.5f,  -0.5f, 0.0f, /** @brief 右下 */
      0.5f,  0.5f,  0.0f  /** @brief 右上 */
  };
  this->_colors = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                   1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
  this->_normals = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  this->_uvs = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f};
  this->_indices = {0, 1, 2, 0, 2, 3};
  // 纹理路径
  this->_text = "";
  // 材质路径
  this->_material = "";
  // 字体大小
  this->_fontSize = 30;
  // 行高
  this->_lineHeight = 30;
}
/**
 * @brief 反序列化组件属性-配置
 * 反序列化成功后，设置文本和材质
 */
void UIText::_deserialized() {
  UIRenderer::_deserialized();
  this->setText(this->_text);
  this->setMaterialAsset(this->_material);
}
void UIText::setText(std::string text) {
  this->_clearTexture();
  this->_text = text;
  Boo::game->fontMgr()->crateFont(this->_fontTexture, this->_text,
                                  this->_fontSize, this->_lineHeight);
  this->_texture = new TextureAsset(this->_fontTexture.uuid);
  this->_texture->create(this->_fontTexture.width, this->_fontTexture.height,
                         this->_fontTexture.channels, this->_fontTexture.datas);
  this->_setTexture(this->_texture);
  this->_updateNodeSize();
}
void UIText::setSize(int fontSize) { this->_fontSize = fontSize; }
void UIText::setLineHeight(int lineHeight) { this->_lineHeight = lineHeight; }
void UIText::setColor(Color &color) {
  if (color.getR() == this->_color.getR() &&
      color.getG() == this->_color.getG() &&
      color.getB() == this->_color.getB() &&
      color.getA() == this->_color.getA()) {
    return;
  }
  this->_setColor(color.getR(), color.getG(), color.getB(), color.getA());
}
void UIText::setColor(std::string color) {
  if (color == this->_color.hexString()) {
    return;
  }
  Color c(color);
  this->_setColor(c.getR(), c.getG(), c.getB(), c.getA());
}
void UIText::setColor(float r, float g, float b, float a) {
  if (r == this->_color.getR() && g == this->_color.getG() &&
      b == this->_color.getB() && a == this->_color.getA()) {
    return;
  }
  this->_setColor(r, g, b, a);
}
void UIText::setAlpha(float alpha) {
  if (alpha == this->_color.getA())
    return;
  Color c(this->_color);
  c.setA(alpha);
  this->_setColor(c.getR(), c.getG(), c.getB(), c.getA());
}

void UIText::setMaterialAsset(std::string mtl) {
  MaterialAsset *mtlAsset =
      dynamic_cast<MaterialAsset *>(Boo::game->assetsManager()->get(mtl));
  if (mtlAsset == nullptr) {
    std::cout << "UISprite::setMaterial: material " << mtl << " not found"
              << std::endl;
    return;
  }
  this->_setMaterial(mtlAsset);
}
void UIText::setMaterialAsset(MaterialAsset *mtl) {
  if (mtl == nullptr) {
    std::cout << "UIText::setMaterialAsset: material is nullptr" << std::endl;
    return;
  }
  this->_setMaterial(mtl);
}

void UIText::_updateRendererState() { UIRenderer::_updateRendererState(); }
/**
 * 更新模型矩阵
 */
void UIText::_updateModelMatrix() { UIRenderer::_updateModelMatrix(); }

void UIText::Update(float deltaTime) { UIRenderer::Update(deltaTime); }
void UIText::LateUpdate(float deltaTime) { UIRenderer::LateUpdate(deltaTime); }
void UIText::Render() { UIRenderer::Render(); }
void UIText::_updateNodeSize() {
  Node2D *node2d = dynamic_cast<Node2D *>(this->_node);
  if (node2d == nullptr) {
    return;
  }
  const Size &size = node2d->getSize();
  float width = this->_fontTexture.width;
  float height = this->_fontTexture.height;
  float scale = size.getHeight() / height;
  node2d->setSize(width * scale, height * scale);
}
void UIText::_clearTexture() {
  if (this->_texture != nullptr) {
    this->_texture->destroy();
    this->_texture = nullptr;
  }
}

void UIText::destroy() { UIRenderer::destroy(); }
UIText::~UIText() {}
