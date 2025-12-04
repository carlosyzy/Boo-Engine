#include "alpha.h"
#include "../../boo.h"
#include "../assets/assets-manager.h"
#include "../gfx/gfx-mgr.h"
#include "../renderer/ui/ui-sprite.h"
#include "../utils/json-util.h"

Alpha::Alpha(const std::string name, const std::string uuid)
    : Scene(name, uuid), _logoAlphaNum(0.0f) {
  this->_alphaDuration = 1.0f;
  this->_logoAlphaNum = 0.0f;
  this->_logoRatio = 0.35f;
  this->_init();
}

void Alpha::_init() {
  this->_initRes();
  this->_initAlpha();
}
void Alpha::_initRes() {
  // this->_textureBg = new TextureAsset("boo-default-texture");
  // this->_textureBg->create(IMAGE_DEFAULT, IMAGE_DEFAULT_SIZE);
  // this->_textureLogo = new TextureAsset("boo-logo-texture");
  // this->_textureLogo->create(IMAGE_LOGO, IMAGE_LOGO_SIZE);
  // this->_logoTxWidth = this->_textureLogo->width();
  // this->_logoTxHeight = this->_textureLogo->height();
  // std::cout << "Alpha::_initRes() logoTxWidth: " << this->_logoTxWidth
  //           << " logoTxHeight: " << this->_logoTxHeight << std::endl;
}
void Alpha::_initAlpha() {
  std::cout << "Alpha::_initAlpha()" << std::endl;
  this->_ndAlpha = new Node2D("Editor-Alpha");
  this->_root2D->addChild(this->_ndAlpha);
  // 添加alpha
  Component *compAlpha = this->_ndAlpha->addComponent("UISprite");
  if (compAlpha != nullptr) {
    this->_spriteAlpha = dynamic_cast<UISprite *>(compAlpha);
    this->_spriteAlpha->setTextureAsset(this->_textureBg);
    this->_spriteAlpha->setMaterialAsset(nullptr);
    this->_spriteAlpha->setColor(0.0f, 0.0f, 1.0f, 1.0f);
  }

  // 添加logo
  this->_ndLogo = new Node2D("Editor-Alpha-Logo");
  this->_ndAlpha->addChild(this->_ndLogo);
  this->_ndLogo->setPosition(0.0f, 100.0f, 0.0f);
  Component *compLogo = this->_ndLogo->addComponent("UISprite");
  if (compLogo != nullptr) {
    this->_spriteLogo = dynamic_cast<UISprite *>(compLogo);
    // Asset *tex =
    // Boo::game->assetsManager()->get("resources/texture/logo.png");
    this->_spriteLogo->setTextureAsset(this->_textureLogo);
    this->_spriteLogo->setMaterialAsset(nullptr);
    this->_spriteLogo->setColor(1.0f, 1.0f, 1.0f, 0.0f);
  }
  // json aa = ReflectionHelper::serializeToJson((void *)compLogo, "UISprite");
  // std::cout << aa << std::endl;
}

void Alpha::update(float deltaTime) {
  // std::cout << "Alpha::update()"<< deltaTime << std::endl;
  Scene::update(deltaTime);
  float _width = Boo::game->view()->width;
  float _height = Boo::game->view()->height;
  if (this->_width != _width || this->_height != _height) {
    this->_width = _width;
    this->_height = _height;
    // Update logo size
    this->_updateBgSize(_width, _height);
    this->_updateLogoSize(_width, _height);
  }

  // Update logo alpha
  this->_updateLogoAlpha(deltaTime);
}
void Alpha::_updateBgSize(float width, float height) {
  if (this->_ndAlpha) {
    this->_ndAlpha->setSize(width, height);
  }
}
void Alpha::_updateLogoSize(float width, float height) {
  if (this->_logoTxWidth == 0.0f || this->_logoTxHeight == 0.0f) {
    return;
  }
  float logoWidth = width * this->_logoRatio;
  float logoHeight = logoWidth * this->_logoTxHeight / this->_logoTxWidth;
  if (this->_ndLogo) {
    this->_ndLogo->setSize(logoWidth, logoHeight);
  }
}

void Alpha::_updateLogoAlpha(float deltaTime) {

  if (this->_spriteLogo == nullptr) {
    return;
  }
  if (this->_logoAlphaNum > this->_alphaDuration) {
    return;
  }
  this->_logoAlphaNum += deltaTime;
  float alpha = this->_logoAlphaNum / this->_alphaDuration;
  this->_spriteLogo->setAlpha(alpha);
}

void Alpha::destroy() { Scene::destroy(); }
Alpha::~Alpha() {}
