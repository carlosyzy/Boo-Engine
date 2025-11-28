#include "assets-manager.h"
#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <unistd.h>
#include <limits.h>
#include <mach-o/dyld.h> // 必需的头文件
#endif
#include "../utils/time-util.h"
#include "asset.h"

AssetsManager::AssetsManager()
{
}
void AssetsManager::init()
{
	this->_assetLoad = new AssetLoad(this);
	this->_cache = new AssetCache();
}
void AssetsManager::initAssetPathMap(const std::string &pathMap)
{
	this->_cache->initAssetPathMap(pathMap);
}
json &AssetsManager::getAssetPathMap()
{
	return this->_cache->getAssetPathMap();
}
void AssetsManager::updateAssetPathMap(const std::string path, json infos)
{
	this->_cache->updateAssetPathMap(path, infos);
}

Asset *AssetsManager::load(const std::string &path)
{
	return this->_assetLoad->load(path);
}
void AssetsManager::clearLoadCall(const int loadId)
{
	this->_assetLoad->clearLoadCall(loadId);
}
Asset *AssetsManager::get(const std::string &path)
{
	return this->_assetLoad->getAsset(path);
}
void AssetsManager::setMaxLoadCount(int count)
{
	this->_assetLoad->setMaxLoadCount(count);
}
void AssetsManager::update(float deltaTime)
{
	this->_assetLoad->update(deltaTime);
}

AssetsManager::~AssetsManager()
{
}

// void AssetsManager::_initRoot()
// {
// #ifdef _WIN32
// 	char exePath[MAX_PATH];
// 	GetModuleFileNameA(NULL, exePath, MAX_PATH);
// 	this->_root = std::filesystem::path(exePath).parent_path().string();
// #elif defined(__APPLE__) && defined(__MACH__)
// #if TARGET_OS_MAC
// 	// macOS 平台
// 	uint32_t size = 0;
// 	_NSGetExecutablePath(nullptr, &size);
// 	std::vector<char> buffer(size);
// 	if (_NSGetExecutablePath(buffer.data(), &size) == 0)
// 	{
// 		try
// 		{
// 			std::string appPath = std::string(buffer.data());
// 			this->_root = std::filesystem::path(appPath).parent_path().string();
// 		}
// 		catch (const std::filesystem::filesystem_error &ex)
// 		{
// 			std::cerr << "文件系统错误: " << ex.what() << std::endl;
// 		}
// 	}
// #elif TARGET_OS_IPHONE
// #if TARGET_IPHONE_SIMULATOR
// 	// iOS 模拟器平台
// #else
// 	// iOS 设备平台
// #endif
// #else
// 	// 其他 Apple 平台
// 	std::cerr << "其他 Apple 平台暂不支持" << std::endl;
// #endif
// #else
// 	std::cerr << "其他平台暂不支持" << std::endl;
// #endif
// 	std::cout << "Assets root:" << this->_root << std::endl;
// }