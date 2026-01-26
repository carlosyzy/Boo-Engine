#include "asset-cache.h"
#include "assets-manager.h"
#include "asset-struct.h"
#include "../utils/file-util.h"
#include "../utils/json-util.h"

AssetCache::AssetCache()
{
   
}
/**
 * @brief 添加资产到缓存
 * @param uuid 资产uuid
 * @param asset 资产指针
 */
void AssetCache::addAsset(const std::string &path, Asset *asset)
{
    this->_assets[path] = asset;
}
/**
 * @brief 获取资产
 * @param path 资产路径
 * @return Asset* 资产指针
 */
Asset *AssetCache::getAsset(const std::string &path)
{
    if (this->_assets.find(path) != this->_assets.end())
    {
        return this->_assets[path];
    }
    return nullptr;
}
AssetCache::~AssetCache()
{
}