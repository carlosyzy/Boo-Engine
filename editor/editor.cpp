#include "editor.h"
#include "boo-editor.h"
#include "cache/editor-assets-cache.h"
#include "cache/editor-config-cache.h"
#include "../engine/boo.h"
#include "../engine/engine.h"
#include "editor-main.h"
#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <unistd.h>
#include <limits.h>
#include <mach-o/dyld.h> // 必需的头文件
#endif

Editor::Editor()
{
	BooEditor::project = "/Users/yangzongyuan/personal/project/Boo-Engine/project";
}
void Editor::init()
{
	Boo::Editor = true;
	BooEditor::assets = new EditorAssetsCache();
	BooEditor::config = new EditorConfigCache();
	this->_initWindow();
	this->_initEngine();
	this->_initEditorMain();
}
void Editor::_initWindow()
{
	this->_window = new Window();
	this->_window->init();
}
void Editor::_initEngine()
{
	this->_engine = new Engine();
#ifdef _WIN32
	this->_engine->init(this->_window, Platform::Windows);
#elif defined(__APPLE__) && defined(__MACH__)
#if TARGET_OS_MAC
	this->_engine->init(this->_window, Platform::MacOS);
#elif TARGET_OS_IPHONE
#if TARGET_IPHONE_SIMULATOR
	// iOS 模拟器平台
#else
	// iOS 设备平台
#endif
#else
	// 其他 Apple 平台
	std::cerr << "其他 Apple 平台暂不支持" << std::endl;
#endif
#else
	std::cerr << "其他平台暂不支持" << std::endl;
#endif
}
void Editor::_initEditorMain()
{
	this->_main = new EditorMain();
	this->_main->init();
}

void Editor::run()
{
	while (this->_window->isRunning())
	{
		this->_window->tick();
		this->_engine->tick();
	}
}

Editor::~Editor()
{
}