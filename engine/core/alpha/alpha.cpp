#include "alpha.h"
#include "boo.h"
#include "log.h"
#include "core/event/event.h"
#include "core/view/view.h"
#include "core/scene/node-2d.h"
#include "core/scene/node-3d.h"
#include "core/renderer/camera.h"
#include "core/renderer/ui/ui-sprite.h"
#include "core/renderer/ui/ui-bmfont-text.h"
#include "core/asset/asset-manager.h"
#include "core/asset/bmfont-asset.h"
#include "core/gfx/gfx-manager.h"

namespace Boo
{

  Alpha::Alpha(const std::string name, const std::string uuid)
      : Scene(name, uuid),
        _width(0),
        _height(0),
        _uiCamera(nullptr),
        _ndAlpha(nullptr),
        _spriteAlpha(nullptr),
        _ndSplash(nullptr),
        _spriteSplash(nullptr),
        _alphaDuration(1.0f),
        _splashAlphaNum(0.0f),
        _isAlphaEnd(false)
  {
    this->_init();
  }

  void Alpha::_init()
  {
    LOGI("[Alpha]: init");
    this->_initCamera();
    this->_initAlpha();
  }
  void Alpha::_initCamera()
  {
    Node2D *node2d = this->getRoot2D();
    Node2D *ndCamera = new Node2D("UI-Camera");
    node2d->addChild(ndCamera);
    ndCamera->setPosition(0.0f, 0.0f, 100.0f);
    this->_uiCamera = dynamic_cast<Camera *>(ndCamera->addComponent("Camera"));
    this->_uiCamera->addGroupID(ENodeGroup::Node2D);
    this->_uiCamera->setProjection(ECameraProjection::Ortho);
  }
  void Alpha::_initAlpha()
  {
    this->_initAlphaBg();
    this->_initAlphaSplash();
  }
  void Alpha::_initAlphaBg()
  {
    // std::cout << "initAlphaBg" << std::endl;
    Node2D *node2d = this->getRoot2D();
    this->_ndAlpha = new Node2D("Alpha");
    this->_ndAlpha->setSize(view->getDesignWidth(), view->getDesignHeight());
    this->_ndAlpha->setPosition(0.0f, 0.0f, 0.0f);
    node2d->addChild(this->_ndAlpha);
    // 添加alpha
    this->_spriteAlpha = dynamic_cast<UISprite *>(this->_ndAlpha->addComponent("UISprite"));
    if (this->_spriteAlpha != nullptr)
    {
      this->_spriteAlpha->setMaterial(AssetBuiltinMaterial::UI);
      this->_spriteAlpha->setColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

  }
  void Alpha::_initAlphaSplash()
  {
    Node2D *node2d = this->getRoot2D();
    // 添加logo splash
    this->_ndSplash = new Node2D("Splash");
    node2d->addChild(this->_ndSplash);
    this->_ndSplash->setPosition(0.0f, 50.0f, 0.0f);
    this->_ndSplash->setSize(85.0f, 85.0f);
    Component *compSplash = this->_ndSplash->addComponent("UISprite");
    if (compSplash != nullptr)
    {
      this->_spriteSplash = dynamic_cast<UISprite *>(compSplash);
      this->_spriteSplash->setMaterial(AssetBuiltinMaterial::UI);
      this->_spriteSplash->setColor(1.0f, 1.0f, 1.0f, 0.0f);
      this->_spriteSplash->setTexture(AssetBuiltinTexture::Splash);
    }

    // 添加bm text测试文本
    Node2D *ndBMText = new Node2D("BMText");
    node2d->addChild(ndBMText);
    ndBMText->setPosition(0.0f, 50.0f, 0.0f);
   
    // UIBMFontText *bmText = dynamic_cast<UIBMFontText *>(ndBMText->addComponent("UIBMFontText"));
    // bmText->setSize(30);
    // bmText->setLineHeight(30);
    // bmText->setText(L"1");
    // bmText->setColor(1.0f, 0.0f, 0.0f, 1.0f);
  }
  void Alpha::update(float deltaTime)
  {
    Scene::update(deltaTime);
    this->_updateLogoAlpha(deltaTime);
  }
  void Alpha::_updateLogoAlpha(float deltaTime)
  {
    if (this->_spriteSplash == nullptr)
    {
      return;
    }
    if (this->_splashAlphaNum > this->_alphaDuration)
    {
      if (this->_isAlphaEnd == false)
      {
        this->_isAlphaEnd = true;
        event->emit(EventEngine_Launch);
      }
      return;
    }
    this->_splashAlphaNum += deltaTime;
    float alpha = this->_splashAlphaNum / this->_alphaDuration;
    this->_spriteSplash->setAlpha(alpha);
  }

  void Alpha::destroy()
  {
    Scene::destroy();
  }
  Alpha::~Alpha() {}

} // namespace Boo
