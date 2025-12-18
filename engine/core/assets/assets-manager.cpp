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
	this->_assetsCache = new AssetCache();
	this->_assetsLoad = new AssetLoad(this);
	this->_initDefaultBuiltinAssets();
}
void AssetsManager::init()
{
}
void AssetsManager::_initDefaultBuiltinAssets()
{
	TextureAsset *texture = new TextureAsset("550e8400-e29b-41d4-a716-446655440000");
	texture->create(DefaultTexture, sizeof(DefaultTexture));
	this->_assetsCache->addAsset("550e8400-e29b-41d4-a716-446655440000", texture);

	TextureAsset *logo = new TextureAsset("123e4567-e89b-12d3-a456-426614174000");
	logo->create(LogoTexture, sizeof(LogoTexture));
	this->_assetsCache->addAsset("123e4567-e89b-12d3-a456-426614174000", logo);
}

void AssetsManager::initAssetsMeta(const std::string &path)
{
	this->_assetsCache->initAssetsMeta(path);
}
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
Asset *AssetsManager::loadAsset(const std::string &uuid)
{
	return this->_assetsLoad->loadAsset(uuid);
}
Asset *AssetsManager::loadAssetByPath(const std::string &path)
{
	return this->_assetsLoad->loadAssetByPath(path);
}
Asset *AssetsManager::getAsset(const std::string &path)
{
	return this->_assetsCache->getAsset(path);
}
Asset *AssetsManager::getAssetByPath(const std::string &path)
{
	return nullptr;
}
/**
 * @brief 获取场景资产数据库
 * @param sceneName 场景名称
 * @return AssetDB 场景资产数据库
 */
json* AssetsManager::_getSceneAssetMate(const std::string &sceneName)
{
	return this->_assetsCache->_getSceneAssetMate(sceneName);
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