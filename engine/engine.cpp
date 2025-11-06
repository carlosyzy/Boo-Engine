#include "engine.h"
#include <iostream>
// #define STB_IMAGE_IMPLEMENTATION
// #include "libs/stb/stb_image.h"
#include "../editor/editor.h"
#include "window/window-mgr.h"
#include "core/gfx/gfx-mgr.h"
// #include "core/font/freetype-mgr.h"
// #include "core/asset/asset-mgr.h"
#include "core/utils/time-util.h"
// #include "core/renderer/scene.h"
// #include "core/global/global.h"
// #include "core/utils/json.h"
// #include "core/input/input-mgr.h"
#include "core/game.h"

Engine::Engine()
{
	this->_frameRate = 30;
}
void Engine::init()
{
	this->_deltaTime = TimeUtil::nowTime();
	// 初始化窗口
	this->_initWindow();
	// 初始化 Vulkan
	this->_initGFX();
	// 初始化Game
	this->_initGame();
	// 初始化编辑器
	this->_initEditor();
}
void Engine::_initWindow()
{
	std::cout << "INIT WINDOW" << std::endl;
	WindowMgr::getInstance()->init();
}
void Engine::_initGFX()
{
	std::cout << "INIT GFX" << std::endl;
	GfxMgr::getInstance()->init();
	GfxMgr::getInstance()->createRenderPass("ui");
}
void Engine::_initGame()
{
	std::cout << "INIT Game MGR" << std::endl;
	Game::getInstance()->init();
}
void Engine::_initEditor()
{
	std::cout << "INIT EDITOR" << std::endl;
	Editor::getInstance()->init();
}
void Engine::launchEditor()
{
	// std::cout << "launch editor" << std::endl;
	// 初始化编辑器

	// std::string project_path = "F:/worksapces/carlos-engine/project";
	// std::string project_path = "/Users/yangzongyuan/personal/project/vulkan/project";
	// Editor::getInstance()->setAssetsRoot(project_path);
	// Editor::getInstance()->setHierarchyRoot(new Scene("Test"));
}
void Engine::launch()
{
	std::cout << "launch engine" << std::endl;
	// 初始化场景
	// this->openScene("Scene");
	// Editor::getInstance()->setScene(this->_curScene);
}

/**
 * @brief 主循环
 */
void Engine::tick()
{
	this->_startRendererThread();
	// 窗口帧循环
	WindowMgr::getInstance()->tick();
	this->_stopRendererThread();
}
/**
 * @brief 启动渲染线程
 */
void Engine::_startRendererThread()
{
	this->_isRenderering = true;
	this->_renderThread = std::thread(&Engine::_updateRendererLoop, this); // 启动渲染线程
}
/**
 * @brief 停止渲染线程
 */
void Engine::_stopRendererThread()
{
	this->_isRenderering = false;
	if (this->_renderThread.joinable())
	{
		this->_renderThread.join(); // 等待线程结束
	}
}
/**
 * @brief 渲染线程循环
 */
void Engine::_updateRendererLoop()
{
	while (this->_isRenderering)
	{
		long long deltaTime = TimeUtil::nowTime();
		long t = deltaTime - this->_deltaTime;
		if (t > 1000 / this->_frameRate)
		{
			this->update(t / 1000.0f);
			this->_deltaTime = deltaTime;
		}
	}
}
/**
 * @brief 引擎更新
 */
void Engine::update(float dt)
{
	int width, height;
	bool isChange = WindowMgr::getInstance()->getWindowSize(width, height);
	if (isChange)
	{
		Game::getInstance()->setView(width, height);
	}
	Editor::getInstance()->update(dt);
	Game::getInstance()->update(dt);
	// 更新渲染器
	GfxMgr::getInstance()->update();
}

Engine::~Engine()
{
}
