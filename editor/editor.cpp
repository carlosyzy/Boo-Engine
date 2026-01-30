#include "editor.h"
#include "boo-editor.h"

#include "core/cache/editor-cache.h"

#include "../engine/boo.h"
#include "../engine/engine.h"
#include "../engine/core/utils/time-util.h"
#include "core/editor-boot.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(__APPLE__)
#include <limits.h>
#include <mach-o/dyld.h> // 必需的头文件
#include <unistd.h>

#endif

Editor::Editor()
{
	this->_deltaTime = TimeUtil::nowTime();
	this->_frameRate = 30;

	Boo::Editor = true;
#if defined(_WIN32) || defined(_WIN64)
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8); // 也设置输入编码为 UTF-8，保持统一

	char exePath[MAX_PATH];
	GetModuleFileNameA(NULL, exePath, MAX_PATH);
	BooEditor::editorPath = std::filesystem::path(exePath).parent_path().string();
	std::cout << "Editor path:" << BooEditor::editorPath << std::endl;
#elif defined(__APPLE__) && defined(__MACH__)
#if TARGET_OS_MAC
	// macOS 平台
	uint32_t size = 0;
	_NSGetExecutablePath(nullptr, &size);
	std::vector<char> buffer(size);
	if (_NSGetExecutablePath(buffer.data(), &size) == 0)
	{
		try
		{
			std::string appPath = std::string(buffer.data());
			BooEditor::editorPath = std::filesystem::path(appPath).parent_path().string();
		}
		catch (const std::filesystem::filesystem_error &ex)
		{
			std::cerr << "file system error: " << ex.what() << std::endl;
		}
	}
	std::cout << "Editor path:" << BooEditor::editorPath << std::endl;
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
void Editor::init()
{

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
	BooEditor::projectPath = "F:\\worksapces\\Boo-Engine\\project";
	this->_engine->init(this->_window, Platform::Windows);
#elif defined(__APPLE__) && defined(__MACH__)
#if TARGET_OS_MAC
	BooEditor::projectPath =
		"/Users/yangzongyuan/personal/project/Boo-Engine/project";
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
	this->_boot = new EditorBoot();
	this->_boot->init();
}

void Editor::run()
{
	while (this->_window->isRunning())
	{
		this->_window->tick();
		this->_tick();
	}
}
void Editor::_tick()
{
	long long deltaTime = TimeUtil::nowTime();
	long long t = deltaTime - this->_deltaTime;
	if (t > (1000.0f / this->_frameRate))
	{
		this->_boot->tick(t / 1000.0f);
		this->_engine->tick(t / 1000.0f);
		this->_deltaTime = deltaTime;
	}
}

Editor::~Editor() {}