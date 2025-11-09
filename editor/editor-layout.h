#pragma once
#include <iostream>
#include <cmath>
class Scene;
class Node2D;
class UISprite;

class EditorLayout
{
private:
	Scene* _scene;

	


	Node2D* _ndMain;
	UISprite* _spriteMain;
	Node2D* _ndMenu;
	UISprite* _spriteMenu;
	Node2D* _ndHierarchy;
	UISprite* _spriteHierarchy;
	Node2D* _ndAsset;
	UISprite* _spriteAsset;
	Node2D* _ndScene;
	UISprite* _spriteScene;
	Node2D* _ndTool;
	UISprite* _spriteTool;
	Node2D* _ndProperty;
	UISprite* _spriteProperty;
	float _border = 2.0f;
	float _width;
	float _height;
	// 菜单布局
	float menu_x;
	float menu_y;
	float menu_width;
	float menu_height;
	// 层级布局
	float hierarchy_x;
	float hierarchy_y;
	float hierarchy_width;
	float hierarchy_height;
	// 资产布局
	float asset_x;
	float asset_y;
	float asset_width;
	float asset_height;
	// 场景布局
	float scene_x;
	float scene_y;
	float scene_width;
	float scene_height;
	// 属性布局
	float property_x;
	float property_y;
	float property_width;
	float property_height;

	// 其他布局
	float tool_x;
	float tool_y;
	float tool_width;
	float tool_height;


	void _initMainUI();
	void _initMenuUI();
	void _initHierarchyUI();
	void _initAssetsUI();
	void _initSceneUI();
	void _initToolUI();
	void _initPropertyUI();
	/**
	 *更新各个模块尺寸
	 */
	void _updateModuleSize();

public:
	EditorLayout();
	Scene* scene() {
		return this->_scene;
	}
	Node2D* getHierarchy() {
		return this->_ndHierarchy;
	}
	Node2D* getAssets() {
		return this->_ndAsset;
	}
	Node2D* getProperty() {
		return this->_ndProperty;
	}

	void update(float dt);
	~EditorLayout();
};