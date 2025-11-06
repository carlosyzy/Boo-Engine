#include "game.h"
#include "gfx/gfx-mgr.h"
#include "global/event.h"
#include "assets/assets-manager.h"



#include "component/component-factory.h"
#include "renderer/ui/ui-sprite.h"
//#include "renderer/ui/sprite.h"
//#include "renderer/ui/text.h"
//#include "ui/ui-widget.h"
//#include "ui/ui-tree/node-tree.h"
//#include "ui/ui-tree/file-tree.h"
//#include "ui/ui-mask.h"
Game::Game():
    _assetsManager(nullptr),
    _curScene(nullptr)
{
	this->_view.width = 1280;
	this->_view.height = 720;
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
	this->_initAssets();
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
	// FreetypeMgr::getInstance()->init();
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
void Game::_initAssets()
{
	std::cout << "INIT ASSETS MGR" << std::endl;
	this->_assetsManager = new AssetsManager();
	this->_assetsManager->init();
}
void Game::setView(int width, int height)
{
	this->_view.width = width;
	this->_view.height = height;
}
void Game::update(float dt)
{
	
}
Game::~Game()
{
}