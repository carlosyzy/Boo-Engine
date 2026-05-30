#include "game.h"
#include "boo.h"
#include "log.h"
#define MINIAUDIO_IMPLEMENTATION
#include "libs/miniaudio/miniaudio.h"
#include "platforms/windows/windows.h"
#include "platforms/macos/macos.h"
#include "platforms/android/android.h"
#include "platforms/harmonyos/harmonyos.h"
#include "core/gfx/gfx-manager.h"
#include "core/font/freetype-mgr.h"
#include "core/alpha/alpha.h"
#include "core/view/view.h"
#include "core/asset/asset-manager.h"
#include "core/event/event.h"
#include "core/input/input.h"
#include "core/profiler/profiler.h"
#include "core/renderer/renderer-mgr.h"
#include "core/util/time-util.h"
#include "core/component/component.h"
#include "core/scene/node.h"

namespace Boo
{

	Game::Game() : _deltaTime(0),
				   _frameRate(30),
				   _viewChanged(false),
				   _viewChangedTime(0.0f),
				   _curScene(nullptr),
				   _scheduleNextID_(0),
				   _windows(nullptr),
				   _macos(nullptr),
				   _android(nullptr),
				   _harmonyos(nullptr)
	{
	}
	void Game::init(Windows *windows, InitConfig &config)
	{
#if defined(BOO_PLATFORM_WINDOWS)
		this->_windows = windows;
		LOGI("[Game]:INIT WINDOW");
		GfxManager::getInstance()->init(windows);
		this->_initAssets();
		this->_initView(windows->getWidth(), windows->getHeight(), config);
		this->_initEvent();
		this->_initInput();
		this->_initAudio();
		this->_initProfiler();
		this->_initRenderer();
		this->_initAlpha();
#endif
	}
	void Game::init(MacOS *macos, InitConfig &config)
	{
#if defined(BOO_PLATFORM_MACOS)
		this->_macos = macos;
		LOGI("[Game]:INIT MACOS");
		GfxManager::getInstance()->init(macos);
		this->_initAssets();
		this->_initView(macos->getWidth(), macos->getHeight(), config);
		this->_initEvent();
		this->_initInput();
		this->_initAudio();
		this->_initProfiler();
		this->_initRenderer();
		this->_initAlpha();
#endif
	}
	void Game::init(Android *android, InitConfig &config)
	{
#if defined(BOO_PLATFORM_ANDROID)
		this->_android = android;
		LOGI("[Game]:INIT ANDROID");
		GfxManager::getInstance()->init(android);
		this->_initEvent();
		this->_initView(android->getWidth(), android->getHeight(), config);
		this->_initAssets();
		this->_initInput();
		this->_initAudio();
		this->_initProfiler();
		this->_initRenderer();
		this->_initAlpha();
#endif
	}
	void Game::init(HarmonyOS *harmonyos, InitConfig &config)
	{
#if defined(BOO_PLATFORM_HARMONYOS)
		this->_harmonyos = harmonyos;
		LOGI("[Game]:INIT HARMONYOS");
		GfxManager::getInstance()->init(harmonyos);
		this->_initEvent();
		this->_initView(harmonyos->getWidth(), harmonyos->getHeight(), config);
		this->_initAssets();
		this->_initInput();
		this->_initAudio();
		this->_initProfiler();
		this->_initRenderer();
		this->_initAlpha();
#endif
	}
	void Game::_initView(int width, int height, InitConfig &config)
	{
		view = new View(width, height);
		view->setDesignFitMode(config.fitMode);
		view->setDesignSize(config.designWidth, config.designHeight);
	}
	void Game::_initAssets()
	{
		LOGI("[Game]:INIT ASSETS MGR");
		assetManager = new AssetManager();
		// 初始化资产管理器
		assetManager->init();
	}
	void Game::_initAudio()
	{
		LOGI("[Game]:INIT AUDIO MGR");
		audioManager = new AudioManager();
		audioManager->init();
	}
	void Game::_initEvent()
	{
		LOGI("[Game]:INIT EVENT");
		event = new Event();
	}
	void Game::_initInput()
	{
		LOGI("[Game]:INIT INPUT");
		input = new Input();
		input->init();
	}
	/**
	 * @brief 初始化性能分析系统
	 */
	void Game::_initProfiler()
	{
		LOGI("[Game]:INIT PROFILER");
		profiler = new Profiler();
		profiler->init();
	}
	void Game::_initRenderer()
	{
		LOGI("[Game]:INIT RENDERER");
		RendererMgr::getInstance()->init();
	}
	void Game::_initAlpha()
	{
		LOGI("[Game]:INIT ALPHA");
		Alpha *alpha = new Alpha("AlphaScene");
		this->openScene(alpha);
	}
	Windows *Game::getWindows()
	{
		return this->_windows;
	}
	MacOS *Game::getMacOS()
	{
		return this->_macos;
	}
	Android *Game::getAndroid()
	{
		return this->_android;
	}
	HarmonyOS *Game::getHarmonyOS()
	{
		return this->_harmonyos;
	}
	void Game::resizeView(const int width, const int height)
	{
		if (view->getWidth() == width && view->getHeight() == height)
		{
			return;
		}
		LOGI("[Game]:resize view: %d %d", width, height);
		view->resize(width, height);
		this->_viewChanged = true;
		this->_viewChangedTime = TimeUtil::nowTime();
		GfxManager::getInstance()->resize(width, height);
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
		LOGI("[Game]:OPEN SCENE: %s", scene->getName().c_str());
		if (scene == nullptr)
		{
			return;
		}
		this->_curScene = scene;
		this->_curScene->launch();
	}
	void Game::destroyScene()
	{
		if (this->_curScene)
		{
			LOGI("[Game]:destroy scene: %s", this->_curScene->getName().c_str());
			this->_curScene->destroy();
			this->_curScene = nullptr;
			RendererMgr::getInstance()->clearCameras();
		}
	}
	Scene *Game::getScene()
	{
		return this->_curScene;
	}
	void Game::addCompClearCaches(Component *comp)
	{
		this->_compClearCaches.push_back(comp);
	}
	void Game::addNodeClearCaches(Node *node)
	{
		this->_nodeClearCaches.push_back(node);
	}
	void Game::setFrameRate(int frameRate)
	{
		this->_frameRate = frameRate;
	}
	const int Game::getFrameRate() const
	{
		return this->_frameRate;
	}
	const int Game::getFps() const
	{
		return this->_fps;
	}
	void Game::tick()
	{

		if (this->_deltaTime == 0)
		{
			this->_deltaTime = TimeUtil::nowTime();
			return;
		}
		long long deltaTime = TimeUtil::nowTime();
		long long t = deltaTime - this->_deltaTime;
		if (t > (1000.0f / this->_frameRate))
		{
			float dt = t / 1000.0f;
			this->_fps = static_cast<int>(std::ceil(1.0f / dt));
			this->_deltaTime = deltaTime;
			this->_update(dt);
			this->_updateSystem(dt);
			this->_lateUpdate(dt);
			this->_render(dt);
			this->_clear();
		}
	}
	void Game::_update(float dt)
	{
		if (this->_curScene)
		{
			this->_curScene->update(dt);
		}
		this->_updateSchedules(dt);
		if (assetManager != nullptr)
		{
			assetManager->update(dt);
		}
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
	void Game::_updateSystem(float dt)
	{
		if (audioManager != nullptr)
		{
			audioManager->update(dt);
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
		// 重置相机渲染纹理大小
		if (this->_viewChanged)
		{
			if (TimeUtil::nowTime() - this->_viewChangedTime >= 100)
			{
				this->_viewChanged = false;
				RendererMgr::getInstance()->updateViewSize();
			}
		}
		// 更新渲染器
		RendererMgr::getInstance()->render(this->_curScene); // 可以统计出渲染物体数量
		if (profiler != nullptr)
		{
			profiler->render();
		}
		GfxManager::getInstance()->update(dt);
	}

	void Game::_clear()
	{
		if (this->_curScene)
		{
			this->_curScene->clearNodeFrameFlag();
		}
		this->_updateClearCaches();
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
		for (auto comp : this->_compClearCaches)
		{
			// std::cout << "Game::_updateClearCaches: clear comp: " << comp->getName() << std::endl;
			// LOGI("Game::_updateClearCaches: clear comp: %s",comp->getName().c_str());
			if (comp != nullptr)
			{
				delete comp;
			}
			comp = nullptr;
		}
		this->_compClearCaches.clear();
		// 清除节点缓存
		for (auto node : this->_nodeClearCaches)
		{
			// std::cout << "Game::_updateClearCaches: clear node: " << node << "   " << node->getName() << std::endl;
			const std::string name = node->getName();
			// LOGI("Game::_updateClearCaches: clear node: %s",name.c_str());
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
	 * @param action 事件动作 GLFW_PRESS(0): 按下 GLFW_RELEASE(1): 释放
	 * @param mods 按键修饰符 GLFW_MOD_SHIFT: Shift GLFW_MOD_CTRL: Ctrl GLFW_MOD_ALT: Alt
	 */
	void Game::updateMouseState(int button, int action, int mods)
	{
		input->_emitMouseButton(button, action, mods);
	}
	void Game::updateMousePos(double xpos, double ypos)
	{
		input->_emitMouseMove(xpos, ypos);
	}
	void Game::updateKeyState(int key, int scancode, int action, int mods)
	{
		// input->_onKey(key, scancode, action, mods);
	}
	void Game::updateScroll(double xoffset, double yoffset)
	{
		input->_emitScroll(xoffset, yoffset);
	}
	void Game::updateTouch(int action, float x, float y)
	{
		input->_emitTouch(action, x, y);
	}

	Game::~Game()
	{
	}

} // namespace Boo