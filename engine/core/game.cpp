#include "game.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libs/stb/stb_image_write.h"

#include "assets/assets-manager.h"
#include "event/event.h"
#include "scene/scene.h"
#include "gfx/gfx-mgr.h"
#include "alpha/alpha.h"
#include "font/freetype-mgr.h"
#include "input/input.h"
#include "utils/time-util.h"

#include "utils/json-util.h"
#include "utils/file-util.h"

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
	this->_initAssets();
	this->_initAlpha();
	// json scene;

	// scene["data"] = {
	// 	{"type", "Scene"},
	// 	{"children", json::array()}};
	// scene["data"]["children"].push_back({{{{"type", "Node"},
	// 									   {"name", "3D"}}},
	// 									 {{{"type", "Node"},
	// 									   {"name", "2D"}}}});
	// FileUtil::saveJsonToBinary(scene, "resources/scene.json");
}
void Game::_initGFX()
{
	std::cout << "INIT GFX" << std::endl;
	GfxMgr::getInstance()->init();
	// GfxMgr::getInstance()->createRenderPass("ui");
}
void Game::_initEvent()
{
	this->_event = new Event();
}
void Game::_initInput()
{
	this->_input = new Input();
	this->_input->init();
}
void Game::_initView()
{
	this->_view = new View();
	this->_view->width = 1280;
	this->_view->height = 720;
}
void Game::_initFont()
{
	this->_fontMgr = new FreetypeMgr();
	this->_fontMgr->init();
}
void Game::_initAssets()
{
	std::cout << "INIT ASSETS MGR" << std::endl;
	this->_assetsManager = new AssetsManager();
	this->_assetsManager->init();
	// 加载必加资源
	// this->_assetsManager->load("resources/texture/logo.png");
	// this->_assetsManager->load("resources/texture/ic-default.png");
	// this->_assetsManager->load("resources/shader/ui/ui.vert.spv");
	// this->_assetsManager->load("resources/shader/ui/ui.frag.spv");
	// this->_assetsManager->load("resources/shader/ui/ui-mask.vert.spv");
	// this->_assetsManager->load("resources/shader/ui/ui-mask.frag.spv");
}
void Game::_initAlpha()
{
	Alpha *alpha = new Alpha("Editor-Alpha");
	this->openScene(alpha);
}

void Game::setView(const int width, const int height)
{
	std::cout << "setView: width:" << width << " height:" << height << std::endl;
	if (this->_view->width == width && this->_view->height == height)
	{
		return;
	}
	this->_view->isFlag = true;
	this->_view->width = width;
	this->_view->height = height;
}
/**
 * @brief 取消调度
 * 手动直接取消指定ID的调度
 * @param scheduleID 调度ID
 */
void Game::unschedule(int scheduleID)
{
	this->_schedules[scheduleID].clearFlag = true;
	this->_scheduleClearCaches.push_back(scheduleID);
}

void Game::openScene(Scene *scene)
{
	// 销毁之前的场景
	this->destroyScene();
	std::cout << "openScene: scene: " << scene->getName() << std::endl;
	this->_curScene = scene;
	this->_curScene->setActive(true);
}
void Game::destroyScene()
{
	if (this->_curScene)
	{
		this->_curScene->destroy();
		this->_curScene = nullptr;
	}
}

void Game::addCompClearCaches(Component *comp)
{
	// std::cout << "Game::addCompClearCaches: comp: " << comp->getNode()->getName() << std::endl;
	this->_compClearCaches.push_back(comp);
}
void Game::addNodeClearCaches(Node *node)
{
	// std::cout << "Game::addNodeClearCaches: node: " << node->getName() << std::endl;
	this->_nodeClearCaches.push_back(node);
}

void Game::tick(float dt)
{

	long long start = TimeUtil::nowTime();
	this->_update(dt);
	this->_lateUpdate(dt);
	this->_logicTime = TimeUtil::nowTime() - start;
	start = TimeUtil::nowTime();
	this->_render(dt);
	this->_renderTime = TimeUtil::nowTime() - start;
	this->_clear();
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
	// 更新渲染器
	GfxMgr::getInstance()->update();
}
void Game::_clear()
{
	if (this->_curScene)
	{
		this->_curScene->clearNodeFrameFlag();
	}
	this->_updateClearCaches();
	this->_view->isFlag = false;
}

void Game::_updateSchedules(float dt)
{
	// 待修复
	for (auto it = this->_schedules.begin(); it != this->_schedules.end();++it)
	{
		if (it->second.clearFlag)
		{
			// 已经标记清除，跳过
			continue;
		}
		ScheduleInfo &info = it->second; 
		if (info.instance == nullptr || info.func == nullptr)
		{
			// 实例或函数为空，标记清除
			info.clearFlag = true;
			this->_scheduleClearCaches.push_back(it->first);
			continue;
		}
		info.time += dt;
		if (info.time >= info.interval)
		{
			info.time = 0.0f;
			info.func();
			bool isOnce = info.isOnce;
			if (isOnce)
			{
				// 一次性调度，清除
				info.clearFlag = true;
				this->_scheduleClearCaches.push_back(it->first);
			}
		}
	}
}
void Game::_updateClearCaches()
{
	// 清除计数器
	for (auto &scheduleId : this->_scheduleClearCaches)
	{
		this->_schedules.erase(scheduleId);
	}
	this->_scheduleClearCaches.clear();

	// 清除组件缓存
	for (auto &comp : this->_compClearCaches)
	{
		std::cout << "Game::_updateClearCaches: clear comp: " << comp->getNode()->getName() << std::endl;
		if (comp != nullptr)
		{
			delete comp;
		}
		comp = nullptr;
	}
	this->_compClearCaches.clear();
	// 清除节点缓存
	for (auto &node : this->_nodeClearCaches)
	{
		std::cout << "Game::_updateClearCaches: clear node: " << node << "   " << node->getName() << std::endl;
		if (node != nullptr)
		{
			delete node;
		}
		node = nullptr;
	}
	this->_nodeClearCaches.clear();
}

void Game::updateMouseState(int button, int action, int mods)
{
	this->_input->onMouseButton(button, action, mods);
}
void Game::updateMousePos(double xpos, double ypos)
{
	this->_input->onCursorPos(xpos, ypos);
}
void Game::updateKeyState(int key, int scancode, int action, int mods)
{
	this->_input->onKey(key, scancode, action, mods);
}

Game::~Game()
{
}