#include "asset-load.h"
#include "asset.h"
#include "texture.h"
#include <filesystem>
#include <iostream>
#include "shader.h"
#include "assets-manager.h"
#include "asset-cache.h"
#include "asset-task.h"

AssetLoad::AssetLoad(AssetsManager *mgr)
{
    this->_mgr = mgr;
    this->_cache = new AssetCache();
}
Asset *AssetLoad::load(const std::string path)
{
    // 从缓存中获取资产
    Asset *asset = this->_cache->getAsset(path);
    if (asset != nullptr)
    {
        return asset;
    }

    // 创建加载任务
    AssetTask task(this->_mgr, this->_cache);
    return task.load(path);
}
Asset *AssetLoad::getAsset(const std::string &path)
{
     Asset *asset = this->_cache->getAsset(path);
    if (asset != nullptr)
    {
        return asset;
    }
    return nullptr;
}



AssetLoad::~AssetLoad()
{
}













// Asset *AssetLoad::load(const std::string path, const std::string fullPath)
// {

// }