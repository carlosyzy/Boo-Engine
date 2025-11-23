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
	Boo::game->init();
	Boo::game->setView(window->getWidth(), window->getHeight());
	this->_deltaTime = TimeUtil::nowTime();
}
/**
 * @brief 引擎更新
 */
void Engine::tick()
{
	if (Boo::game == nullptr)
		return;
	
	long long deltaTime = TimeUtil::nowTime();
	long long t = deltaTime - this->_deltaTime;
	if (t > (1000.0f / this->_frameRate))
	{
		
		Boo::game->tick(t / 1000.0f);
		this->_deltaTime = deltaTime;
	}
}

Engine::~Engine()
{
}
