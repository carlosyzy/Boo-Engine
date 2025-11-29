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
	this->_cache = new AssetCache();
	this->_assetLoad = new AssetLoad(this);
}
void AssetsManager::initAssetsDB(const std::string &path)
{
	this->_cache->initAssetsDB(path);
}
/**
 * @brief 获取资产数据库
 * @return std::unordered_map<std::string, std::vector<AssetDB>> 资产数据库
 */
std::unordered_map<std::string, std::vector<AssetDB>> &AssetsManager::getAssetsDB()
{
	return this->_cache->getAssetsDB();
}
/**
 * @brief 更新资产数据库
 * @param path 资产路径
 * @param configs 资产配置
 */
void AssetsManager::updateAssetsDB(const std::string path, const std::vector<AssetDB> configs)
{
	this->_cache->updateAssetsDB(path, configs);
}
void AssetsManager::setAssetsRoot(const std::string &root)
{
	this->_assetsRoot = root;
}
const std::string &AssetsManager::getAssetsRoot()
{
	return this->_assetsRoot;
}
AssetLoad *AssetsManager::assetLoad()
{
	return this->_assetLoad;
}
AssetCache *AssetsManager::assetCache()
{
	return this->_cache;
}
/**
 * @brief 加载资产
 * @param path 资产路径 相对于assets 目录的路径
 * @return Asset* 资产指针
 */
Asset *AssetsManager::loadByUuid(const std::string &uuid)
{
	return this->_assetLoad->loadByUuid(uuid);
}
void AssetsManager::clearLoadCall(const int loadId)
{
	// this->_assetLoad->clearLoadCall(loadId);
}
Asset *AssetsManager::getAssetByUuid(const std::string &path)
{
	return this->_cache->getAssetByUuid(path);
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

// }