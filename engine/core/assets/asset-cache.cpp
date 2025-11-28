#include "asset-cache.h"
#include "assets-manager.h"

AssetCache::AssetCache()
{
    
}
void AssetCache::addAsset(const std::string path, Asset *asset)
{
    this->_assetsMap[path] = asset;
}

Asset *AssetCache::getAsset(const std::string &path)
{
    if(this->_assetsMap.find(path) == this->_assetsMap.end()){
        return nullptr;
    }
    return this->_assetsMap[path];
}
void AssetCache::addSceneAsset(const std::string path, Asset *asset)
{
    this->_sceneAssetsMap[path] = asset;
}
Asset *AssetCache::getSceneAsset(const std::string &path)
{
    if(this->_sceneAssetsMap.find(path) == this->_sceneAssetsMap.end()){
        return nullptr;
    }
    return this->_sceneAssetsMap[path];
}



AssetCache::~AssetCache()
{
}