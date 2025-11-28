#include "asset-cache.h"
#include "assets-manager.h"
#include "../utils/file-util.h"
#include "../utils/json-util.h"

AssetCache::AssetCache()
{
    
}
void AssetCache::initAssetPathMap(const std::string &pathMap)
{
    this->_pathAssetMap = FileUtil::readJsonFromBinary(pathMap);
}
json &AssetCache::getAssetPathMap()
{
    return this->_pathAssetMap;
}
void AssetCache::updateAssetPathMap(const std::string path, json infos)
{
    this->_pathAssetMap[path] = infos;
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