#pragma once

#include "window/window.h"
#include "core/game.h"
/**
 * @brief 平台枚举
 */
enum class Platform
{
	None,
	Windows,
	MacOS,
	Android,
};


namespace Boo
{
	extern Game* game;
	extern Window* window;
	extern Platform platform;
}