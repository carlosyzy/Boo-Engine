#include "editor-layout.h"
#include <filesystem>
#include <iostream>
#include "../boo-editor.h"
#include "../config/editor-config.h"


#include "../../engine/boo.h"
#include "../../engine/core/game.h"
#include "../../engine/core/input/input.h"
#include "../../engine/core/scene/scene.h"
#include "../../engine/core/scene/node-2d.h"
#include "../../engine/core/renderer/camera.h"
#include "../../engine/core/renderer/ui/ui-sprite.h"
#include "../../engine/core/renderer/ui/ui-mask.h"
#include "../../engine/core/assets/assets-manager.h"
#include "../../engine/core/component/ui/ui-widget.h"

EditorLayout::EditorLayout(std::string name, Node *node, std::string uuid) : Component(name, node, uuid)
{
    
}
/**
 * @brief 反序列化组件属性-配置
 * 反序列化成功后，更新模块尺寸
 */
void EditorLayout::_deserialized()
{
    Component::_deserialized();
}
void EditorLayout::Awake()
{
    this->_initCamera();
    this->_initMainUI();
    this->_initMenuUI();
    this->_initHierarchyUI();
    this->_initAssetsUI();
    this->_initPropertyUI();
    this->_initSceneUI();
    this->_initToolUI();
    this->_initBottomUI();
}
void EditorLayout::Enable()
{
    Component::Enable();
}
void EditorLayout::_initCamera()
{
	Scene *scene = Boo::game->getScene();
	if (scene == nullptr)
	{
		return;
	}
	Node2D *node2d = scene->getRoot2D();
	Node2D *ndCamera = new Node2D("Editor-EditorLoading-Camera");
	node2d->addChild(ndCamera);
	ndCamera->setPosition(0.0f, 0.0f, -100.0f);
	this->_uiCamera = dynamic_cast<Camera *>(ndCamera->addComponent("Camera"));
}

void EditorLayout::_initMainUI()
{
    std::cout << "EditorLayout::_initMainUI" << std::endl;
    this->_ndMain = new Node2D("Editor-Main");
    this->_ndMain->setSize(this->_width, this->_height);
    this->_node->addChild(this->_ndMain);
    this->_spriteMain = dynamic_cast<UISprite *>(this->_ndMain->addComponent("UISprite"));
    if (this->_spriteMain != nullptr)
    {
        std::cout << "EditorLayout::_initMainUI1" << std::endl;
        this->_spriteMain->setColor(0.1f, 0.1f, 0.1f, 1.0f);
    }
}
void EditorLayout::_initMenuUI()
{
    std::cout << "EditorLayout::_initMenuUI" << std::endl;
    this->_ndMenu = new Node2D("Editor-Menu");
    this->_ndMenu->setSize(this->menu_width, this->menu_height);
    this->_ndMenu->setPosition(this->menu_x, this->menu_y, 0.0f);
    this->_ndMain->addChild(this->_ndMenu);
    // 渲染组件-后期不需要
    this->_spriteMenu = dynamic_cast<UISprite *>(this->_ndMenu->addComponent("UISprite"));
    if (this->_spriteMenu != nullptr)
    {
        this->_spriteMenu->setColor(EditorConfig::theme);
    }
}
void EditorLayout::_initHierarchyUI()
{
    this->_ndHierarchy = new Node2D("Editor-EditorHierarchy");
    this->_ndMain->addChild(this->_ndHierarchy);
    this->_ndHierarchy->setSize(this->hierarchy_width, this->hierarchy_height);
    this->_ndHierarchy->setPosition(this->hierarchy_x, this->hierarchy_y, 0.0f);
    this->_spriteHierarchy = dynamic_cast<UISprite *>(this->_ndHierarchy->addComponent("UISprite"));
    if (this->_spriteHierarchy != nullptr)
    {
        this->_spriteHierarchy->setColor(EditorConfig::theme);
    }
}
void EditorLayout::_initAssetsUI()
{
    this->_ndAsset = new Node2D("Editor-Asset");
    this->_ndMain->addChild(this->_ndAsset);
    this->_ndAsset->setSize(this->asset_width, this->asset_height);
    this->_ndAsset->setPosition(this->asset_x, this->asset_y, 0.0f);
    this->_spriteAsset = dynamic_cast<UISprite *>(this->_ndAsset->addComponent("UISprite"));
    if (this->_spriteAsset != nullptr)
    {
        this->_spriteAsset->setColor("#0A2F36");
    }
}
void EditorLayout::_initPropertyUI()
{
    this->_ndProperty = new Node2D("Editor-Property");
    this->_ndMain->addChild(this->_ndProperty);
    this->_ndProperty->setSize(this->property_width, this->property_height);
    this->_ndProperty->setPosition(this->property_x, this->property_y, 0.0f);
    this->_spriteProperty = dynamic_cast<UISprite *>(this->_ndProperty->addComponent("UISprite"));
    if (this->_spriteProperty != nullptr)
    {
        this->_spriteProperty->setColor(EditorConfig::theme);
    }
}
void EditorLayout::_initSceneUI()
{
    this->_ndScene = new Node2D("Editor-Scene");
    this->_ndMain->addChild(this->_ndScene);
    this->_ndScene->setSize(this->scene_width, this->scene_height);
    this->_ndScene->setPosition(this->scene_x, this->scene_y, 0.0f);
    this->_spriteScene = dynamic_cast<UISprite *>(this->_ndScene->addComponent("UISprite"));
    if (this->_spriteScene != nullptr)
    {
        this->_spriteScene->setColor(EditorConfig::theme);
    }
}
void EditorLayout::_initToolUI()
{
    this->_ndTool = new Node2D("Editor-Tool");
    this->_ndMain->addChild(this->_ndTool);
    this->_ndTool->setSize(this->tool_width, this->tool_height);
    this->_ndTool->setPosition(this->tool_x, this->tool_y, 0.0f);
    this->_spriteTool = dynamic_cast<UISprite *>(this->_ndTool->addComponent("UISprite"));
    if (this->_spriteTool != nullptr)
    {
        this->_spriteTool->setColor(EditorConfig::theme);
    }
}
void EditorLayout::_initBottomUI()
{
    this->_ndBottom = new Node2D("Editor-Bottom");
    this->_ndMain->addChild(this->_ndBottom);
    this->_ndBottom->setSize(this->bottom_width, this->bottom_height);
    this->_ndBottom->setPosition(this->bottom_x, this->bottom_y, 0.0f);
    this->_spriteBottom = dynamic_cast<UISprite *>(this->_ndBottom->addComponent("UISprite"));
    if (this->_spriteBottom != nullptr)
    {
        this->_spriteBottom->setColor(EditorConfig::theme);
    }
}

void EditorLayout::Update(float dt)
{
    Component::Update(dt);
    View *view = Boo::game->view();
    if (this->_width != view->width || this->_height != view->height)
    {
        this->_width = view->width;
        this->_height = view->height;
        std::cout << "EditorLayout::update:" << this->_width << " " << this->_height << std::endl;
        this->_updateModuleSize();
    }
};
void EditorLayout::LateUpdate(float dt)
{
    Component::LateUpdate(dt);
}


void EditorLayout::_updateModuleSize()
{
    // 基于左上角为0，0点的坐标
    //  菜单
    this->menu_width = this->_width;
    this->menu_height = 25;
    this->menu_x = 0.0f;
    this->menu_y = this->_height / 2.0 - this->menu_height / 2.0;

    // 底部
    this->bottom_width = this->_width;
    this->bottom_height = 25;
    this->bottom_x = 0.0f;
    this->bottom_y = -this->_height / 2.0 + this->bottom_height / 2.0;

    // 层级
    this->hierarchy_width = std::ceil(this->_width * 0.25);
    this->hierarchy_height = std::ceil((this->_height - this->menu_height - this->bottom_height) * 0.55);
    this->hierarchy_x = -this->_width / 2.0 + this->hierarchy_width / 2.0;
    this->hierarchy_y = std::ceil(this->_height / 2.0 - this->menu_height - this->_border - this->hierarchy_height / 2.0);
    // 资产
    this->asset_width = this->hierarchy_width;
    this->asset_height = this->_height - this->menu_height - this->_border - this->hierarchy_height - this->_border - this->bottom_height - this->_border;
    this->asset_x = -this->_width / 2.0 + this->asset_width / 2.0;
    this->asset_y = std::ceil(this->_height / 2.0 - this->menu_height - this->_border - this->hierarchy_height - this->_border - this->asset_height / 2.0);

    // 属性
    this->property_width = std::ceil(this->_width * 0.25);
    this->property_height = this->_height - this->menu_height - this->_border - this->bottom_height - this->_border;
    this->property_x = std::round(this->_width / 2.0 - this->property_width / 2.0);
    this->property_y = std::ceil(this->_height / 2.0 - this->menu_height - this->_border - this->property_height / 2.0);

    // 场景
    this->scene_width = std::round(this->_width - this->hierarchy_width - this->property_width - this->_border - this->_border);
    this->scene_height = this->hierarchy_height;
    this->scene_x = std::round(-this->_width / 2.0 + this->hierarchy_width + this->_border + this->scene_width / 2.0);
    this->scene_y = std::round(this->_height / 2.0 - this->menu_height - this->_border - this->scene_height / 2.0);
    // 工具
    this->tool_width = this->scene_width;
    this->tool_height = this->asset_height;
    this->tool_x = this->scene_x;
    this->tool_y = this->asset_y;

    if (this->_ndMain != nullptr)
    {
        this->_ndMain->setSize(this->_width, this->_height);
        this->_ndMain->setPosition(0.0f, 0.0f, 0.0f);
    }
    if (this->_ndMenu != nullptr)
    {
        this->_ndMenu->setSize(this->menu_width, this->menu_height);
        this->_ndMenu->setPosition(this->menu_x, this->menu_y, 0.0f);
    }
    if (this->_ndHierarchy != nullptr)
    {
        this->_ndHierarchy->setSize(this->hierarchy_width, this->hierarchy_height);
        this->_ndHierarchy->setPosition(this->hierarchy_x, this->hierarchy_y, 0.0f);
    }
    if (this->_ndAsset != nullptr)
    {
        this->_ndAsset->setSize(this->asset_width, this->asset_height);
        this->_ndAsset->setPosition(this->asset_x, this->asset_y, 0.0f);
    }
    if (this->_ndProperty != nullptr)
    {
        this->_ndProperty->setSize(this->property_width, this->property_height);
        this->_ndProperty->setPosition(this->property_x, this->property_y, 0.0f);
    }
    if (this->_ndScene != nullptr)
    {
        this->_ndScene->setSize(this->scene_width, this->scene_height);
        this->_ndScene->setPosition(this->scene_x, this->scene_y, 0.0f);
    }

    if (this->_ndTool != nullptr)
    {
        this->_ndTool->setSize(this->tool_width, this->tool_height);
        this->_ndTool->setPosition(this->tool_x, this->tool_y, 0.0f);
    }
    if (this->_ndBottom != nullptr)
    {
        this->_ndBottom->setSize(this->bottom_width, this->bottom_height);
        this->_ndBottom->setPosition(this->bottom_x, this->bottom_y, 0.0f);
    }
}
void EditorLayout::Disable()
{
    Component::Disable();
}
void EditorLayout::destroy()
{
    Component::destroy();
}


EditorLayout::~EditorLayout()
{
    std::cout << "EditorLayout::~EditorLayout()" << std::endl;
}