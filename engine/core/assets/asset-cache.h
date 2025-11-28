#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include <functional>
#include "../../core/utils/json-util.h"

enum class AssetType;
class Asset;
class AssetsManager;
/**
 * @brief 资产缓存类
 */
class AssetCache
{
private:
    /**
     * @brief 路径资产映射数据
     */
    json _pathAssetMap;

    /**
     * @brief 资产映射表
     */
    std::unordered_map<std::string, Asset *> _assetsMap;
    std::unordered_map<std::string, Asset *> _sceneAssetsMap;

public:
    AssetCache();

    /**
     * @brief 初始化资产路径映射
     * @param pathMap 资产路径映射
     */
    void initAssetPathMap(const std::string &pathMap);
    /**
     * @brief 获取资产路径映射
     * @return json 资产路径映射
     */
    json &getAssetPathMap();
    /**
     * @brief 更新资产路径映射
     * @param path 资产路径
     * @param infos 资产信息
     */
    void updateAssetPathMap(const std::string path, json infos);



    /**
     * @brief 添加资产到缓存
     *
     * @param path 资产路径
     * @param asset 资产指针
     */
    void addAsset(const std::string path, Asset *asset);
    Asset *getAsset(const std::string &path);

    

    void addSceneAsset(const std::string path, Asset *asset);
    Asset *getSceneAsset(const std::string &path);
    ~AssetCache();
};
