#include "game.h"

Game::Game() : _assetsManager(nullptr),
			   _curScene(nullptr)
{
}

void Game::init()
{
	this->_initGFX();
	this->_initEvent();
	this->_initInput();
	this->_initView();
	this->_initFont();
	this->_initComponents();
	this->_initAssets();
	this->_initAlpha();
}
void Game::_initGFX()
{
	std::cout << "INIT GFX" << std::endl;
	GfxMgr::getInstance()->init();
	GfxMgr::getInstance()->createRenderPass("ui");
}
void Game::_initEvent()
{
	this->_event = new Event();
}
void Game::_initInput()
{
	/* InputMgr::getInstance()->init();*/
}
void Game::_initView()
{
	this->_view = new View();
	this->_view->width = 1280;
	this->_view->height = 720;
}
void Game::_initFont()
{
	// FreetypeMgr::getInstance()->init();
}

void Game::_initComponents()
{
	this->registerComponentClass<UISprite>("UISprite");
	this->registerComponentClass<UIWidget>("UIWidget");
	this->registerComponentClass<NodeTree>("NodeTree");
	this->registerComponentClass<UILayoutHorizontal>("UILayoutHorizontal");
	/*
	  this->_componentFactory->registerClass<Text>("Text");

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
void Game::_initAlpha()
{
	Alpha *alpha = new Alpha("Editor-Alpha");
	this->_curScene = static_cast<Scene *>(alpha);
	// 启动场景
}

void Game::setView(int width, int height)
{
	this->_view->width = width;
	this->_view->height = height;
}

void Game::unschedule(int scheduleID)
{
	this->_schedules.erase(scheduleID);
}
/**
 * @brief 创建组件
 * 
 * @param className 组件类名
 * @param node 节点
 * @param uuid 组件UUID
 * @return Component* 组件指针
 */
Component *Game::createComponent(const std::string &className, Node *node, std::string uuid)
{
	auto it = this->_creatorComponentMap.find(className);
	if (it != this->_creatorComponentMap.end())
	{
		return it->second(node, uuid);
	}

	std::cout << "Component class not found: " << className << std::endl;
	return nullptr;
}
void Game::addCompClearCaches(Component *comp)
{
	std::cout << "Game::addCompClearCaches: comp: " << comp->getNode()->getName() << std::endl;
	this->_compClearCaches.push_back(comp);
}
void Game::addNodeClearCaches(Node *node)
{
	std::cout << "Game::addNodeClearCaches: node: " << node->getName() << std::endl;
	this->_nodeClearCaches.push_back(node);
}

void Game::update(float dt)
{
	this->_update(dt);
	this->_lateUpdate(dt);
	this->_render(dt);
	this->_clear();
	// 更新渲染器
	GfxMgr::getInstance()->update();
}
void Game::_update(float dt)
{
	if (this->_curScene)
	{
		this->_curScene->update(dt);
	}
	this->_updateSchedules(dt);
	if (this->_assetsManager)
	{
		this->_assetsManager->update(dt);
	}
}
void Game::_lateUpdate(float dt)
{
	if (this->_curScene)
	{
		this->_curScene->lateUpdate(dt);
	}
}
void Game::_render(float dt)
{
	if (this->_curScene)
	{
		this->_curScene->render();
	}
}
void Game::_clear()
{
	if (this->_curScene)
	{
		this->_curScene->clearNodeFrameFlag();
	}
	this->_updateClearCaches();
}

void Game::_updateSchedules(float dt)
{
	for (auto it = this->_schedules.begin(); it != this->_schedules.end();)
	{
		ScheduleInfo &info = it->second;
		if (info.instance == nullptr)
		{
			it = this->_schedules.erase(it);
			continue;
		}
		info.time += dt;
		if (info.time >= info.interval)
		{
			info.time = 0.0f;
			info.func();
			bool isOnce = info.isOnce;
			int currentId = it->first; // 保存当前ID
			if (isOnce)
			{
				it = this->_schedules.find(currentId);
				if (it != this->_schedules.end())
				{
					it = this->_schedules.erase(it);
				}
				else
				{
					// 如果已经被删除，需要重新获取有效的迭代器
					it = this->_schedules.begin();
					if (it == this->_schedules.end())
						break;
				}
			}
			else
			{
				++it;
			}
		}
		else
		{
			++it;
		}
	}
}
void Game::_updateClearCaches()
{
	for (auto &comp : this->_compClearCaches)
	{
		std::cout << "Game::_updateClearCaches: clear comp: " << comp->getNode()->getName() << std::endl;
		delete comp;
		comp = nullptr;
	}
	this->_compClearCaches.clear();
	for (auto &node : this->_nodeClearCaches)
	{
		std::cout << "Game::_updateClearCaches: clear node: " << node->getName() << std::endl;
		delete node;
		node = nullptr;
	}
	this->_nodeClearCaches.clear();
}
Game::~Game()
{
}