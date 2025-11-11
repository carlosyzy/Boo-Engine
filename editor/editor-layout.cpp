#include "editor-layout.h"
#include <filesystem>
#include <iostream>
#include "editor-config.h"

#include "../engine/core/game.h"
#include "../engine/core/scene/scene.h"
#include "../engine/core/scene/node-2d.h"
#include "../engine/core/renderer/ui/ui-sprite.h"
#include "../engine/core/assets/assets-manager.h"
#include "../engine/core/assets/asset.h"
#include "../engine/core/assets/texture.h"
// #include "../engine/core/input/input-mgr.h"

EditorLayout::EditorLayout()
{
	// 创建
	this->_scene = new Scene("EDITOR");
	this->_initLoadUI();
}
void EditorLayout::_initLoadUI()
{
	Node2D *root2D = this->_scene->getRoot2D();
	this->_ndLoad = new Node2D("Editor-Load");
	root2D->addChild(this->_ndLoad);
	this->_spriteLoad = dynamic_cast<UISprite *>(this->_ndLoad->addComponent("UISprite"));
	if (this->_spriteLoad != nullptr)
	{
		this->_spriteLoad->setTexture("resources/texture/ic-default.png");
		this->_spriteLoad->setColor(1.0, 1.0, 1.0, 1.0f);
		this->_spriteLoad->setMaterial(nullptr);
		this->_spriteLoad->setColor("#0A2F36");
	}
	this->_ndLoadBar = new Node2D("Editor-LoadBar");
	this->_ndLoad->addChild(this->_ndLoadBar);
	this->_spriteLoadBar = dynamic_cast<UISprite *>(this->_ndLoadBar->addComponent("UISprite"));
	if (this->_spriteLoadBar != nullptr)
	{
		this->_spriteLoadBar->setTexture("resources/texture/ic-default.png");
		this->_spriteLoadBar->setMaterial(nullptr);
		this->_spriteLoadBar->setColor(1.0f, 1.0f, 1.0f, 1.0f);
		this->_spriteLoadBar->setColor("#AFF2FF");
	}
}
void EditorLayout::setLoadProgress(float progress)
{
	this->_loadProgress = progress;
	if (this->_ndLoad != nullptr && this->_ndLoadBar != nullptr)
	{
		float loadWidth = this->_ndLoad->getSize().getWidth();
		float loadBarWidth = loadWidth * this->_loadProgress;
		this->_ndLoadBar->setSize(loadBarWidth, 5.0f);
		this->_ndLoadBar->setPosition(-loadWidth / 2.0 + loadBarWidth / 2.0, 0.0f, 0.0f);
	}
}
void EditorLayout::launch()
{
	if (this->_ndLoad !=nullptr) {
		this->_ndLoad->destroy();
	}
}

// InputMgr::getInstance()->setRoot(root2D);
/*this->_initMainUI();*/
/*this->_initMenuUI();
this->_initHierarchyUI();
this->_initAssetsUI();
this->_initSceneUI();
this->_initToolUI();
this->_initPropertyUI();*/

void EditorLayout::_initMainUI()
{
	this->_ndMain = new Node2D("Editor-Main");
	this->_scene->getRoot2D()->addChild(this->_ndMain);
	Component *comp = this->_ndMain->addComponent("UISprite");
	if (comp != nullptr)
	{
		this->_spriteMain = dynamic_cast<UISprite *>(comp);
		Asset *tex = Game::getInstance()->assetsManager()->get("F:/worksapces/Boo-Engine/x64/Debug/res/ic-default.png");
		this->_spriteMain->setTexture(dynamic_cast<Texture *>(tex));
		this->_spriteMain->setMaterial(nullptr);
		this->_spriteMain->setColor(1.1f, 0.1f, 0.1f, 1.0f);
	}
}
void EditorLayout::_initMenuUI()
{
	this->_ndMenu = new Node2D("Editor-Menu");
	this->_scene->getRoot2D()->addChild(this->_ndMenu);
	Component *comp = this->_ndMenu->addComponent("UISprite");
	if (comp != nullptr)
	{
		this->_spriteMenu = dynamic_cast<UISprite *>(comp);
		this->_spriteMenu->setColor(0.0, 0.0, 0.0, 1.0f);
	}
}
void EditorLayout::_initHierarchyUI()
{
	this->_ndHierarchy = new Node2D("Editor-EditorHierarchy");
	this->_scene->getRoot2D()->addChild(this->_ndHierarchy);
	Component *comp = this->_ndHierarchy->addComponent("UISprite");
	if (comp != nullptr)
	{
		this->_spriteHierarchy = dynamic_cast<UISprite *>(comp);
		this->_spriteHierarchy->setColor(0.0, 0.0, 0.0, 1.0f);
	}
}
void EditorLayout::_initAssetsUI()
{
	this->_ndAsset = new Node2D("Editor-Asset");
	this->_scene->getRoot2D()->addChild(this->_ndAsset);
	Component *comp = this->_ndAsset->addComponent("UISprite");
	if (comp != nullptr)
	{
		this->_spriteAsset = dynamic_cast<UISprite *>(comp);
		this->_spriteAsset->setColor(0.0, 0.0, 0.0, 1.0f);
	}
}
void EditorLayout::_initSceneUI()
{
	this->_ndScene = new Node2D("Editor-Scene");
	this->_scene->getRoot2D()->addChild(this->_ndScene);
	Component *comp = this->_ndScene->addComponent("UISprite");
	if (comp != nullptr)
	{
		this->_spriteScene = dynamic_cast<UISprite *>(comp);
		this->_spriteScene->setColor(0.0, 0.0, 0.0, 1.0f);
	}
}
void EditorLayout::_initToolUI()
{
	this->_ndTool = new Node2D("Editor-Tool");
	this->_scene->getRoot2D()->addChild(this->_ndTool);
	Component *comp = this->_ndTool->addComponent("UISprite");
	if (comp != nullptr)
	{
		this->_spriteTool = dynamic_cast<UISprite *>(comp);
		this->_spriteTool->setColor(0.0, 0.0, 0.0, 1.0f);
	}
}
void EditorLayout::_initPropertyUI()
{
	this->_ndProperty = new Node2D("Editor-Property");
	this->_scene->getRoot2D()->addChild(this->_ndProperty);
	Component *comp = this->_ndProperty->addComponent("UISprite");
	if (comp != nullptr)
	{
		this->_spriteProperty = dynamic_cast<UISprite *>(comp);
		this->_spriteProperty->setColor(0.0, 0.0, 0.0, 1.0f);
	}
}
void EditorLayout::update(float dt)
{
	View &view = Game::getInstance()->view();
	if (this->_width != view.width || this->_height != view.height)
	{
		this->_width = view.width;
		this->_height = view.height;
		this->_updateModuleSize();
	}
	if (this->_scene != nullptr)
	{
		this->_scene->update(dt);
		this->_scene->lateUpdate(dt);
		this->_scene->render();
		this->_scene->clearNodeFrameFlag();
	}
};

void EditorLayout::_updateModuleSize()
{
	// 基于左上角为0，0点的坐标
	//  菜单
	this->menu_width = this->_width;
	this->menu_height = 25;
	this->menu_x = 0.0f;
	this->menu_y = 0.0f;
	// 层级
	this->hierarchy_x = 0.0f;
	this->hierarchy_y = std::ceil(this->menu_y + this->menu_height + this->_border);
	this->hierarchy_width = std::ceil(this->_width * 0.25);
	this->hierarchy_height = std::ceil((this->_height - this->menu_height) * 0.55);
	// 资产
	this->asset_x = 0.0f;
	this->asset_y = std::ceil(this->hierarchy_y + this->hierarchy_height + this->_border);
	this->asset_width = this->hierarchy_width;
	this->asset_height = this->_height - this->hierarchy_y - this->hierarchy_height - this->_border;
	// 属性
	this->property_width = std::ceil(this->_width * 0.25);
	this->property_y = std::ceil(this->menu_y + this->menu_height + this->_border);
	this->property_x = std::round(this->_width - this->property_width);
	this->property_height = this->_height - this->menu_y - this->menu_height - this->_border;
	// 场景
	this->scene_x = std::round(this->hierarchy_x + this->hierarchy_width + this->_border);
	this->scene_y = std::round(this->menu_y + this->menu_height + this->_border);
	this->scene_width = std::round(this->_width - this->scene_x - this->property_width - this->_border);
	this->scene_height = this->hierarchy_height;
	// 工具
	this->tool_x = this->scene_x;
	this->tool_y = this->asset_y;
	this->tool_width = this->scene_width;
	this->tool_height = this->asset_height;

	if (this->_ndLoad != nullptr)
	{
		float loadWidth = this->_width * 0.55;
		this->_ndLoad->setSize(loadWidth, 5.0f);
		this->_ndLoad->setPosition(0.0, -this->_height / 2 + this->_height / 2 * 0.4, 0.0f);
		if (this->_ndLoadBar != nullptr)
		{
			float loadBarWidth = loadWidth * this->_loadProgress;
			this->_ndLoadBar->setSize(loadBarWidth, 5.0f);
			this->_ndLoadBar->setPosition(-loadWidth / 2.0 + loadBarWidth / 2.0, 0.0f, 0.0f);
		}
	}

	if (this->_ndMain != nullptr)
	{
		this->_ndMain->setSize(this->_width, this->_height);
		this->_ndMain->setPosition(0.0f, 0.0f, 0.0f);
	}
	if (this->_ndMenu != nullptr)
	{
		this->_ndMenu->setSize(this->menu_width, this->menu_height);
		this->_ndMenu->setPosition(-this->_width / 2.0 + this->menu_x + this->menu_width / 2.0, this->_height / 2.0 - this->menu_y - this->menu_height / 2.0, 0.0f);
		// std::cout << "x:" << (-this->_width / 2.0 + this->menu_x + this->menu_width / 2.0) << std::endl;
	}
	if (this->_ndHierarchy != nullptr)
	{
		this->_ndHierarchy->setSize(this->hierarchy_width, this->hierarchy_height);
		this->_ndHierarchy->setPosition(-this->_width / 2.0 + this->hierarchy_x + this->hierarchy_width / 2.0, this->_height / 2.0 - this->hierarchy_y - this->hierarchy_height / 2.0, 0.0f);
	}
	if (this->_ndAsset != nullptr)
	{
		this->_ndAsset->setSize(this->asset_width, this->asset_height);
		this->_ndAsset->setPosition(-this->_width / 2.0 + this->asset_x + this->asset_width / 2.0, this->_height / 2.0 - this->asset_y - this->asset_height / 2.0, 0.0f);
	}
	if (this->_ndScene != nullptr)
	{
		this->_ndScene->setSize(this->scene_width, this->scene_height);
		this->_ndScene->setPosition(-this->_width / 2.0 + this->scene_x + this->scene_width / 2.0, this->_height / 2.0 - this->scene_y - this->scene_height / 2.0, 0.0f);
	}

	if (this->_ndTool != nullptr)
	{
		this->_ndTool->setSize(this->tool_width, this->tool_height);
		this->_ndTool->setPosition(-this->_width / 2.0 + this->tool_x + this->tool_width / 2.0, this->_height / 2.0 - this->tool_y - this->tool_height / 2.0, 0.0f);
	}
	if (this->_ndProperty != nullptr)
	{
		this->_ndProperty->setSize(this->property_width, this->property_height);
		this->_ndProperty->setPosition(-this->_width / 2.0 + this->property_x + this->property_width / 2.0, this->_height / 2.0 - this->property_y - this->property_height / 2.0, 0.0f);
	}
}
EditorLayout::~EditorLayout()
{
	std::cout << "EditorLayout::~EditorLayout()" << std::endl;
}