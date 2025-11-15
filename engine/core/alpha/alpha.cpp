#include "alpha.h"
#include "../../boo.h"
#include "../assets/assets-manager.h"
#include "../renderer/ui/ui-sprite.h"

Alpha::Alpha(const std::string name, const std::string uuid) : Scene(name, uuid), _logoAlphaNum(0.0f)
{
	this->_alphaDuration = 2.0f;
	this->_logoAlphaNum = 0.0f;
	this->_logoRatio = 0.35f;
	this->_init();
}

void Alpha::_init()
{
	this->_initRes();
	this->_initAlpha();
	this->_initDelayScheduleID=Boo::game->scheduleOnce(&Alpha::_onAlphaAnimOK,this, this->_alphaDuration/2.0);
}
void Alpha::_initRes()
{
	Boo::game->assetsManager()->load("resources/texture/logo.png");
	Boo::game->assetsManager()->load("resources/texture/ic-default.png");
	Boo::game->assetsManager()->load("resources/shader/ui/ui.vert.spv");
	Boo::game->assetsManager()->load("resources/shader/ui/ui.frag.spv");
	Texture* textureLogo = static_cast<Texture*>(Boo::game->assetsManager()->get("resources/texture/logo.png"));
	if (textureLogo != nullptr)
	{
		this->_logoTxWidth = textureLogo->width();
		this->_logoTxHeight = textureLogo->height();
	}
}
void Alpha::_initAlpha()
{
	std::cout << "Alpha::_initAlpha()" << std::endl;
	this->_ndAlpha = new Node2D("Editor-Alpha");
	this->_root2D->addChild(this->_ndAlpha);
	// 添加logo
	this->_ndLogo = new Node2D("Editor-Alpha-Logo");
	this->_ndAlpha->addChild(this->_ndLogo);
	this->_ndLogo->setPosition(0.0f, 100.0f, 0.0f);
	Component* compLogo = this->_ndLogo->addComponent("UISprite");
	if (compLogo != nullptr)
	{
		this->_spriteLogo = dynamic_cast<UISprite*>(compLogo);
		// Asset *tex = Boo::game->assetsManager()->get("resources/texture/logo.png");
		this->_spriteLogo->setTexture("resources/texture/logo.png");
		this->_spriteLogo->setMaterial(nullptr);
		this->_spriteLogo->setColor(1.0f, 1.0f, 1.0f, 0.0f);
	}
	this->_updateLogoSize();
}

void Alpha::_onAlphaAnimOK()
{
	
}


void Alpha::update(float deltaTime)
{
	Scene::update(deltaTime);
	// Update logo alpha
	this->_updateLogoAlpha(deltaTime);
	// Update logo size
	this->_updateLogoSize();
}

void Alpha::_updateLogoAlpha(float deltaTime)
{
	if (this->_spriteLogo == nullptr)
	{
		return;
	}
	if (this->_logoAlphaNum > this->_alphaDuration)
	{
		return;
	}
	this->_logoAlphaNum += deltaTime;
	this->_spriteLogo->setAlpha(this->_logoAlphaNum / this->_alphaDuration);
}
void Alpha::_updateLogoSize()
{
	if (this->_spriteLogo == nullptr)
	{
		return;
	}
	if (this->_logoTxWidth == 0.0f || this->_logoTxHeight == 0.0f)
	{
		return;
	}
	float _width = Boo::game->view()->width;
	float _height = Boo::game->view()->height;
	if (this->_width == _width && this->_height == _height)
	{
		return;
	}
	this->_width = _width;
	this->_height = _height;
	float width = this->_width * this->_logoRatio;
	float height = width * this->_logoTxHeight / this->_logoTxWidth;
	this->_ndLogo->setSize(width, height);
}

void Alpha::destroy()
{
	Scene::destroy();
	Boo::game->unschedule(this->_initDelayScheduleID);
}
Alpha::~Alpha()
{
}
