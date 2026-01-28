#include "engine.h"
#include <iostream>

#include "boo.h"
#include "window/window.h"
#include "core/utils/time-util.h"


Engine::Engine()
{
	this->_deltaTime = TimeUtil::nowTime();
	this->_frameRate = 30;
}
void Engine::init(Window *window, Platform platform)
{
	Boo::platform = platform;
	Boo::window = window;
	Boo::game = new Game();
	Boo::game->init(window->getWidth(), window->getHeight());
	this->_deltaTime = TimeUtil::nowTime();
}
/**
 * @brief 引擎更新
 */
void Engine::tick(float deltaTime)
{
	if (Boo::game == nullptr)
		return;
	Boo::game->tick(deltaTime);
}

Engine::~Engine()
{
}
