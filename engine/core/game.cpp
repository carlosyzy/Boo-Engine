#include "game.h"
#include "gfx/gfx-mgr.h"
#include "global/event.h"

#include "component/component-factory.h"
#include "renderer/ui/ui-sprite.h"
//#include "renderer/ui/sprite.h"
//#include "renderer/ui/text.h"
//#include "ui/ui-widget.h"
//#include "ui/ui-tree/node-tree.h"
//#include "ui/ui-tree/file-tree.h"
//#include "ui/ui-mask.h"
Game::Game()
{
}
Game* Game::getInstance()
{
	static Game instance;
	return &instance;
}
void Game::init()
{
	this->_initEvent();
	this->_initInput();
	this->_initFont();
	this->_initComponents();
}

void Game::_initEvent() {
	this->_event = new Event();
}
void Game::_initInput()
{
	/* InputMgr::getInstance()->init();*/
}
void Game::_initFont()
{
	//FreetypeMgr::getInstance()->init();
}

void Game::_initComponents()
{
	this->_componentFactory = new ComponentFactory();
	this->_componentFactory->registerClass<UISprite>("UISprite");
	/* 
	  this->_componentFactory->registerClass<Text>("Text");
	  this->_componentFactory->registerClass<UIWidget>("UIWidget");
	  this->_componentFactory->registerClass<NodeTree>("NodeTree");
	  this->_componentFactory->registerClass<FileTree>("FileTree");
	  this->_componentFactory->registerClass<UIMask>("UIMask");*/
}
void Game::setView(int width, int height)
{
	this->_view.width = width;
	this->_view.height = height;
}
void Game::update(float dt)
{


	// 更新渲染器
	GfxMgr::getInstance()->update();
}
Game::~Game()
{
}