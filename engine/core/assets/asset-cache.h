#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <functional>
#include "../../core/utils/json-util.h"

enum class AssetType;
class Asset;
class TextureAsset;
class AssetsManager;

struct AssetDB
{
    std::string name;
    std::string path;
    std::string uuid;
    std::string extension;
    AssetType type;
};

/**
 * @brief 资产缓存类
 */
class AssetCache
{
private:
    /**
     * @brief 资产数据库
     * @param path 资产路径
     * @return std::vector<AssetDB> 资产配置
     */
    std::unordered_map<std::string, std::vector<AssetDB>> _assetsDB;
    /**
     * @brief 资产数据库
     * @param uuid 资产uuid
     * @return std::vector<AssetDB> 资产配置
     */
    std::unordered_map<std::string, AssetDB> _uuidsDB;


    /**
     * @brief 资产映射表
     */
    std::unordered_map<std::string, Asset *> _assetsUuidMap;
    
    std::unordered_map<std::string, Asset *> _sceneAssetsMap;

public:
    AssetCache();
    /**
     * @brief 初始化资产数据库
     * @param path 资产数据库路径
     */
    void initAssetsDB(const std::string &path);
    /**
     * @brief 获取资产数据库
     * @return std::unordered_map<std::string, std::vector<AssetDB>> 资产数据库
     */
    std::unordered_map<std::string, std::vector<AssetDB>> &getAssetsDB();
    /**
     * @brief 更新资产数据库
     * @param path 资产路径
     * @param configs 资产配置
     */
    void updateAssetsDB(const std::string path, const std::vector<AssetDB> configs);
    /**
     * @brief 获取资产配置
     * @param path 资产路径
     * @return std::vector<AssetDB> 资产配置
     */
    const std::vector<AssetDB> &getAssetDB(const std::string &path);
    /**
     * @brief 获取资产配置
     * @param uuid 资产uuid
     * @return AssetDB 资产配置
     */
    const AssetDB &getAssetDBByUuid(const std::string &uuid);

    // void addAsset(const std::string &uuid, Asset *asset);
    Asset *getAssetByUuid(const std::string &uuid);

    /**
     * @brief 添加资产到缓存
     *
     * @param uuid 资产uuid
     * @param asset 资产指针
     */
    void addAssetByUuid(const std::string &uuid, Asset *asset);

    // /**
    //  * @brief 添加资产到缓存
    //  *
    //  * @param path 资产路径
    //  * @param asset 资产指针
    //  */
    // void addAsset(const std::string path, Asset *asset);
    // Asset *getAsset(const std::string &path);

    // void addSceneAsset(const std::string path, Asset *asset);
    // Asset *getSceneAsset(const std::string &path);
    ~AssetCache();
};
