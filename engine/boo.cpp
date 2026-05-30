#include "boo.h"
#include <iostream>
namespace Boo
{
	Game *game = nullptr;
	/**
	 * @brief 事件系统
	 */
	Event *event = nullptr;
	/**
	 * @brief 视图系统
	 */
	View *view = nullptr;
	/**
	 * @brief 资产系统
	 */
	AssetManager *assetManager = nullptr;
	/**
	 * @brief 性能分析系统
	 */
	Profiler *profiler = nullptr;
	/**
	 * @brief 输入系统
	 */
	Input *input = nullptr;
	/**
	 * @brief 音频系统
	 */
	AudioManager *audioManager = nullptr;
}