#include "asset-manager.h"
#include "log.h"
#include "core/asset/asset-load.h"
#include "core/asset/asset-builtin.h"

namespace Boo
{

	AssetManager::AssetManager() : _assetsLoad(new AssetLoad()),
									 _assetsCache(new AssetCache()),
									 _assetsBuiltin(new AssetBuiltin())

	{
	}
	void AssetManager::init()
	{
		this->_assetsBuiltin->init();
	}
	const std::string &AssetManager::getAssetsRoot()
	{
		return this->_assetsRoot;
	}

	AssetLoad *AssetManager::getAssetsLoad()
	{
		return this->_assetsLoad;
	}
	AssetCache *AssetManager::getAssetsCache()
	{
		return this->_assetsCache;
	}
	void AssetManager::setMaxLoadCount(int count)
	{
	}
	Asset *AssetManager::loadAsset(const std::string &path)
	{
		return this->_assetsLoad->loadAsset(path);
	}

	Asset *AssetManager::getAsset(const std::string &path, bool loadIfNotFound)
	{
		if (loadIfNotFound) {
			return this->_assetsLoad->loadAsset(path);
		}
		return this->_assetsCache->getAsset(path);
	}
	void AssetManager::unloadAsset(const std::string &path)
	{
		LOGI("The current function is for implementation"); // 当前功能为实现,后续会根据需求进行修改
	}
	void AssetManager::update(float deltaTime)
	{
		this->_assetsLoad->update(deltaTime);
	}

	AssetManager::~AssetManager()
	{
		delete this->_assetsLoad;
		delete this->_assetsCache;
		delete this->_assetsBuiltin;
	}

} // namespace Boo