#include "assets-manager.h"
#include "asset.h"
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <unistd.h>
#include <limits.h>
#include <mach-o/dyld.h> // 必需的头文件
#endif
#include <iostream>
#include "asset-load.h"

AssetsManager::AssetsManager()
{
}
void AssetsManager::init()
{
	this->_initRoot();
}
void AssetsManager::_initRoot()
{
#ifdef _WIN32
	char exePath[MAX_PATH];
	GetModuleFileNameA(NULL, exePath, MAX_PATH);
	this->_root = std::filesystem::path(exePath).parent_path().string();
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
			this->_root = std::filesystem::path(appPath).parent_path().string();
		}
		catch (const std::filesystem::filesystem_error &ex)
		{
			std::cerr << "文件系统错误: " << ex.what() << std::endl;
		}
	}
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
	std::cout << "Assets root:" << this->_root << std::endl;
}
Asset *AssetsManager::load(const std::string &path)
{
	std::filesystem::path fullPath = std::filesystem::path(this->_root) / path;
	Asset *asset = AssetLoad::load(path, fullPath.string());
	if (asset != nullptr)
	{
		this->_assetsMap[path] = asset;
		return asset;
	}
	return nullptr;
}
// void AssetsManager::loadAsync(const std::string &path, std::function<void(Asset *)> callback)
// {
// }

AssetsManager::~AssetsManager()
{
}
