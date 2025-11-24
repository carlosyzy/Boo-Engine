#include "editor-loading.h"
#include "../../boo.h"
#include "../../../engine/core/renderer/ui/ui-sprite.h"
#include "../../../engine/core/assets/assets-manager.h"
// #include "../renderer/ui/ui-sprite.h"
// #include "../gfx/gfx-mgr.h"
// #include "../gfx/gfx-pipeline-struct.h"

EditorLoading::EditorLoading(const std::string name, const std::string uuid) : Scene(name, uuid)
{
	this->_logoRatio = 0.35f;
	this->_width = Boo::game->view()->width;
	this->_height = Boo::game->view()->height;
	this->_initBg();
	this->_initLogo();
}
void EditorLoading::_initBg()
{
	this->_ndAlpha = new Node2D("Editor-EditorLoading");
	this->_root2D->addChild(this->_ndAlpha);
	// 背景
	Component *compAlpha = this->_ndAlpha->addComponent("UISprite");
	if (compAlpha != nullptr)
	{
		this->_spriteAlpha = dynamic_cast<UISprite *>(compAlpha);
		this->_spriteAlpha->setTexture("resources/texture/alpha.png");
		this->_spriteAlpha->setMaterial(nullptr);
		this->_spriteAlpha->setColor(0.2f, 0.0f, 1.0f, 1.0f);
	}
	this->_ndAlpha->setSize(this->_width, this->_height);
}
void EditorLoading::_initLogo()
{
	// 添加logo
	this->_ndLogo = new Node2D("Editor-EditorLoading-Logo");
	this->_root2D->addChild(this->_ndLogo);
	this->_ndLogo->setPosition(0.0f, 100.0f, 0.0f);
	Component *compLogo = this->_ndLogo->addComponent("UISprite");
	if (compLogo != nullptr)
	{
		this->_spriteLogo = dynamic_cast<UISprite *>(compLogo);
		this->_spriteLogo->setTexture("resources/texture/logo.png");
		this->_spriteLogo->setMaterial(nullptr);
		this->_spriteLogo->setColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	Texture *textureLogo = static_cast<Texture *>(Boo::game->assetsManager()->get("resources/texture/logo.png"));
	if (textureLogo != nullptr)
	{
		this->_logoTxWidth = textureLogo->width();
		this->_logoTxHeight = textureLogo->height();
	}
	// 初始化logo大小
	this->_updateLogoSize(this->_width, this->_height);
}

void EditorLoading::update(float deltaTime)
{
	Scene::update(deltaTime);
	float width = Boo::game->view()->width;
	float height = Boo::game->view()->height;
	if (this->_width != width || this->_height != height)
	{
		this->_width = width;
		this->_height = height;
		std::cout << "EditorLoading::update() width: " << width << " height: " << height << std::endl;
		// Update logo size
		this->_updateBgSize(width, height);
		this->_updateLogoSize(width, height);
	}
}
void EditorLoading::_updateBgSize(float width, float height)
{
	if (this->_ndAlpha)
	{
		this->_ndAlpha->setSize(width, height);
	}
}
void EditorLoading::_updateLogoSize(float width, float height)
{
	if (this->_logoTxWidth == 0.0f || this->_logoTxHeight == 0.0f)
	{
		return;
	}
	float logoWidth = width * this->_logoRatio;
	float logoHeight = logoWidth * this->_logoTxHeight / this->_logoTxWidth;
	if (this->_ndLogo)
	{
		this->_ndLogo->setSize(logoWidth, logoHeight);
		std::cout << "EditorLoading::_updateLogoSize() logoWidth: " << logoWidth << " logoHeight: " << logoHeight << std::endl;
	}
}

void EditorLoading::destroy()
{
	Scene::destroy();
}
EditorLoading::~EditorLoading()
{
}
