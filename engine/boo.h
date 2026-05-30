#pragma once
#include "core/game.h"
#include "core/event/event.h"
#include "core/view/view.h"
#include "core/asset/asset-manager.h"
#include "core/profiler/profiler.h"
#include "core/input/input.h"
#include "core/audio/audio-manager.h"
namespace Boo
{
	class Game;
	class Event;
	class View;
	class Profiler;
	class Input;
	class AssetManager;
	class AudioManager;

	extern Game *game;
	/**
	 * @brief 事件系统
	 */
	extern Event *event;
	/**
	 * @brief 视图系统
	 */
	extern View *view;
	/**
	 * @brief 资产系统
	 */
	extern AssetManager *assetManager;
	/**
	 * @brief 性能分析系统
	 */
	extern Profiler *profiler;
	/**
	 * @brief 输入系统
	 */
	extern Input *input;
	/**
	 * @brief 音频系统
	 */
	extern AudioManager *audioManager;

}