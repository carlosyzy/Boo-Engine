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
#include "renderer/renderer.h"
#include "renderer/camera.h"
#include "renderer/ui/ui-renderer.h"

#include "utils/time-util.h"
#include "utils/json-util.h"
#include "utils/file-util.h"

Game::Game() : _assetsManager(nullptr),
			   _curScene(nullptr), _view(nullptr)
{
}

void Game::init(const int width, const int height)
{
	// 初始化视图大小
	this->_view = new View();
	this->_view->width = width;
	this->_view->height = height;
	// 初始化模块
	this->_initGFX();
	this->_initEvent();
	this->_initInput();
	this->_initFont();
	this->_initAssets();
	this->_initRenderer();
}
void Game::_initGFX()
{
	std::cout << "INIT GFX" << std::endl;
	GfxMgr::getInstance()->init();
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
}
void Game::_initRenderer()
{
	this->_renderer = new Renderer();
	this->_renderer->init();
}
void Game::_initAlpha()
{
	std::cout << "INIT ALPHA" << std::endl;
	Alpha *alpha = new Alpha("Editor-Alpha");
	this->openScene(alpha);
}
void Game::resizeView(const int width, const int height)
{
	// std::cout << "resizeView: width:" << width << " height:" << height << std::endl;
	if (this->_view->width == width && this->_view->height == height)
	{
		return;
	}
	this->_view->width = width;
	this->_view->height = height;
	this->_viewChanged = true;
	this->_viewChangedTime = TimeUtil::nowTime();
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
	if (scene == nullptr)
	{
		return;
	}
	this->_curScene = scene;
	this->_curScene->setActive(true);
}
void Game::destroyScene()
{
	if (this->_curScene)
	{
		std::cout << "destroyScene: scene: " << this->_curScene->getName() << std::endl;
		this->_cameras.clear();
		this->_curScene->destroy();
		this->_curScene = nullptr;
	}
	std::cout << "destroyScene: scene submit complete: " << std::endl;
}
/**
 * @brief 挂在相机到游戏中
 *
 * @param camera 相机指针
 */
void Game::extractCamera(Camera *camera)
{
	if (this->_cameras.find(camera->getUuid()) != this->_cameras.end())
	{
		return;
	}
	std::cout << "extractCamera: camera: " << camera->getUuid() << std::endl;
	this->_cameras[camera->getUuid()] = camera;
	camera->resize(this->_view->width, this->_view->height);
	std::cout << "extractCamera: camera resize: " << this->_cameras.size() << std::endl;
}
/**
 * @brief 从游戏中移除相机
 *
 * @param camera 相机指针
 */
void Game::removeCamera(Camera *camera)
{
	if (this->_cameras.find(camera->getUuid()) == this->_cameras.end())
	{
		return;
	}
	this->_cameras.erase(camera->getUuid());
	std::cout << "removeCamera: camera: " << camera->getUuid() << std::endl;
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
	if(this->_viewChanged){
		if(TimeUtil::nowTime() - this->_viewChangedTime >= 0.2f){
			this->_viewChanged = false;
			for (auto &camera : this->_cameras)
			{
				camera.second->resize(this->_view->width, this->_view->height);
			}
		}
	}
	// 更新渲染器
	this->_renderer->render(this->_cameras, this->_curScene);
	GfxMgr::getInstance()->update(dt);
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
	// 待修复
	for (auto it = this->_schedules.begin(); it != this->_schedules.end(); ++it)
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
		// std::cout << "Game::_updateClearCaches: clear comp: " << comp->getNode()->getName() << std::endl;
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
		// std::cout << "Game::_updateClearCaches: clear node: " << node << "   " << node->getName() << std::endl;
		if (node != nullptr)
		{
			delete node;
		}
		node = nullptr;
	}
	this->_nodeClearCaches.clear();
}
/**
 * @brief 鼠标按钮事件
 * @param button 鼠标按钮 0: 左键 1: 右键 2: 中键
 * @param action 事件动作 GLFW_PRESS: 按下 GLFW_RELEASE: 释放
 * @param mods 按键修饰符
 */
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