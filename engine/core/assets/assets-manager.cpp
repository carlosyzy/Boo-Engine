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
	this->_assetsCache = new AssetCache();
	this->_assetsLoad = new AssetLoad(this);

	TextureAsset *texture = new TextureAsset("550e8400-e29b-41d4-a716-446655440000");
	texture->create(DefaultTexture, sizeof(DefaultTexture));
	this->_assetsCache->addAsset("550e8400-e29b-41d4-a716-446655440000", texture);

	TextureAsset *logo = new TextureAsset("123e4567-e89b-12d3-a456-426614174000");
	logo->create(LogoTexture, sizeof(LogoTexture));
	this->_assetsCache->addAsset("123e4567-e89b-12d3-a456-426614174000", logo);
}
void AssetsManager::initAssetsDB(const std::string &path)
{
	this->_assetsCache->initAssetsDB(path);
}
// /**
//  * @brief 获取资产数据库
//  * @return std::unordered_map<std::string, std::vector<AssetDB>> 资产数据库
//  */
// std::unordered_map<std::string, std::vector<AssetDB>> &AssetsManager::getAssetsDB()
// {
// 	return this->_cache->getAssetsDB();
// }
// /**
//  * @brief 更新资产数据库
//  * @param path 资产路径
//  * @param configs 资产配置
//  */
// void AssetsManager::updateAssetsDB(const std::string path, const std::vector<AssetDB> configs)
// {

// }

void AssetsManager::setAssetsRoot(const std::string &root)
{
	this->_assetsRoot = root;
}
const std::string &AssetsManager::getAssetsRoot()
{
	return this->_assetsRoot;
}

AssetLoad *AssetsManager::getAssetsLoad()
{
	return this->_assetsLoad;
}
AssetCache *AssetsManager::getAssetsCache()
{
	return this->_assetsCache;
}
void AssetsManager::setMaxLoadCount(int count)
{
	this->_assetsLoad->setMaxLoadCount(count);
}
Asset *AssetsManager::loadByUuid(const std::string &uuid)
{
	return this->_assetsLoad->loadByUuid(uuid);
}
// void AssetsManager::clearLoadCall(const int loadId)
// {
// 	// this->_assetsLoad->clearLoadCall(loadId);
// }
Asset *AssetsManager::getAssetByUuid(const std::string &path)
{
	return this->_assetsCache->getAssetByUuid(path);
}

void AssetsManager::update(float deltaTime)
{
	this->_assetsLoad->update(deltaTime);
}

AssetsManager::~AssetsManager()
{
}

// void AssetsManager::_initRoot()
// {

// }