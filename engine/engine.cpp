#include "engine.h"
#include <iostream>
#include "../editor/editor.h"
#include "window/window.h"
#include "core/utils/time-util.h"
#include "boo.h"

Engine::Engine()
{
	this->_frameRate = 30;
}
void Engine::init()
{
	this->_deltaTime = TimeUtil::nowTime();
	// 初始化窗口
	this->_initWindow();
	// 初始化Game
	this->_initGame();
	// 初始化编辑器
	this->_initEditor();
}
void Engine::_initWindow()
{
	std::cout << "INIT WINDOW" << std::endl;
	Boo::window = new Window(this);
	Boo::window->init();
}

void Engine::_initGame()
{
	std::cout << "INIT Game MGR" << std::endl;
	Boo::game = new Game();
	Boo::game->init();
}
void Engine::_initEditor()
{
	std::cout << "INIT EDITOR" << std::endl;
	Editor::getInstance()->init();
}

/**
 * @brief 主循环
 */
void Engine::run()
{
	while (Boo::window->isRunning())
	{
		Boo::window->tick();
		long long deltaTime = TimeUtil::nowTime();
		long t = deltaTime - this->_deltaTime;
		if (t > 1000.0f / this->_frameRate)
		{
			this->tick(t / 1000.0f);
			this->_deltaTime = deltaTime;
		}
	}
}
/**
 * @brief 引擎更新
 */
void Engine::tick(float dt)
{
	Editor::getInstance()->update(dt);
	if (Boo::game != nullptr)
	{
		Boo::game->tick(dt);
	}
}

void Engine::updateViewSize(float width, float height)
{
	if (Boo::game != nullptr)
	{
		Boo::game->setView(width, height);
	}
}
void Engine::updateMouseState(int button, int action, int mods)
{
	if (Boo::game != nullptr)
	{
		Boo::game->updateMouseState(button, action, mods);
	}
}
void Engine::updateMousePos(double xpos, double ypos)
{
	if (Boo::game != nullptr)
	{
		Boo::game->updateMousePos(xpos, ypos);
	}
}

Engine::~Engine()
{
}
