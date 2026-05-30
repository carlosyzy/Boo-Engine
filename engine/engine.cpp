#include "engine.h"
#include "boo.h"
#include "log.h"
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "libs/stb/stb_image_write.h"
#include "platforms/platform.h"
#include "platforms/windows/windows.h"   //windows下的窗口封装
#include "platforms/android/android.h" //android下的窗口封装
#include "platforms/macos/macos.h"     //macos下的窗口封装
#include "platforms/harmonyos/harmonyos.h" //harmonyos下的窗口封装
#include "core/game.h"

Engine::Engine()
{
	Boo::game = new Boo::Game();
}
void Engine::init(Windows *windows,Boo::InitConfig &config)
{
	LOGI("[Engine]:INIT WINDOW");
	Boo::game->init(windows, config);
}
void Engine::init(Android *android,Boo::InitConfig &config)
{
	LOGI("[Engine]:INIT ANDROID");
	Boo::game->init(android, config);
}
void Engine::init(MacOS *macos,Boo::InitConfig &config)
{
	LOGI("[Engine]:INIT MACOS");
	Boo::game->init(macos, config);
}
void Engine::init(HarmonyOS *harmonyos,Boo::InitConfig &config)
{
	LOGI("[Engine]:INIT HARMONYOS");
	Boo::game->init(harmonyos, config);
}
/**
 * @brief 引擎更新
 */
void Engine::tick()
{
	if (Boo::game == nullptr)
		return;
	Boo::game->tick();
}

Engine::~Engine()
{
}
