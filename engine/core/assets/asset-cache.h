#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <any>
#include <filesystem>
#include <functional>
#include "asset-struct.h"
#include "../../core/utils/json-util.h"

enum class AssetType;
class Asset;
class TextureAsset;
class AssetsManager;

/**
 * @brief 资产缓存类
 */
class AssetCache
{
private:
    /**
     * 全局资产映射(1对1)
     * key: 资产uuid 全局唯一
     * value : 资产实例指针
     */
    std::unordered_map<std::string, Asset *> _assets;
    /**
     * @brief 资产数据库 (1对1)
     *  key: 资产uuid 全局唯一
     *  value: 资产基本信息
     */
    std::unordered_map<std::string, AssetDB *> _assetsDB;
    /**
     * @brief 场景映射数据库(1对1)
     *  场景名称映射资产数据库
     */
    std::unordered_map<std::string, AssetDB *> _sceneAssetsMap;

    /**
     * @brief 资产数据库
     *  key: 资产路径
     *  value: 资产基本信息数组
     */
    std::unordered_map<std::string, std::vector<AssetDB *>> _pathAssetsDB;

    /**
     * @brief 解析纹理资产数据库
     * @param path 资产数据库路径
     * @param assetDB 资产数据库
     */
    void _insertAssetDB(const std::string &path, json assetDB);

public:
    AssetCache();
    /**
     * @brief 初始化资产数据库
     * @param path 资产数据库路径
     */
    void initAssetsDB(const std::string &path);
    /**
     * @brief 通过资产路径获取资产数据库
     * @param path 资产路径
     * @return std::vector<AssetDB *> 资产数据库指针数组
     */
    std::vector<AssetDB *> getAssetDBByPath(const std::string &path);
    /**
     * @brief 更新资产数据库
     * @param path 资产路径
     * @param assetDBs 资产数据库指针数组
     */
    void updateAssetDBByPath(const std::string &path, int index, const AssetDB &assetDB);
    /**
     * @brief 获取资产数据库
     */
    std::unordered_map<std::string, std::vector<AssetDB *>> &getPathAssetsDB();

    /**
     * @brief 添加资产到缓存
     * @param uuid 资产uuid
     * @param asset 资产实例指针
     */
    void addAsset(const std::string &uuid, Asset *asset);
    /**
     * @brief 通过资产uuid获取资产实例指针
     * @param uuid 资产uuid
     * @return Asset* 资产实例指针
     */
    Asset *getAsset(const std::string &uuid);
    /**
     * @brief 通过资产路径获取资产实例指针
     * @param path 资产路径
     * @return Asset* 资产实例指针
     */
    Asset *getAssetByPath(const std::string &path);

    /**
     * @brief 通过资产uuid获取资产数据库
     * @param uuid 资产uuid
     * @return const AssetDB& 资产数据库
     */
    AssetDB *getAssetDB(const std::string &uuid);

    /**
     * @brief 通过场景名称获取场景配置
     * @param sceneName 场景名称
     * @return AssetDB* 场景资产数据库指针
     */
    AssetDB *getSceneAssetDB(const std::string &sceneName);

    // /**
    //  * @brief 获取纹理资产数据库
    //  * @return std::vector<AssetDB> 纹理资产数据库
    //  */
    // const std::unordered_map<std::string, AssetDB> &getTextureAssetsDB();
    // void _updateTextureAssetsDB(const std::string &path, const AssetDB &config);
    // /**
    //  * @brief 获取音频资产数据库
    //  * @return std::vector<AssetDB> 音频资产数据库
    //  */
    // const std::unordered_map<std::string, AssetDB> &getAudioAssetsDB();
    // /**
    //  * @brief 更新音频资产数据库
    //  * @param path 资产路径
    //  * @param configs 资产配置
    //  */
    // void updateAudioAssetsDB(const std::string &path, const AssetDB &config);
    // /**
    //  * @brief 获取材质资产数据库
    //  * @return std::vector<AssetDB> 材质资产数据库
    //  */
    // const std::unordered_map<std::string, AssetDB> &getMaterialAssetsDB();
    // /**
    //  * @brief 更新材质资产数据库
    //  * @param path 资产路径
    //  * @param configs 资产配置
    //  */
    // void updateMaterialAssetsDB(const std::string &path, const AssetDB &config);

    // /**
    //  * @brief 获取资产数据库
    //  * @return std::unordered_map<std::string, std::vector<AssetDB>> 资产数据库
    //  */
    // std::unordered_map<std::string, std::vector<AssetDB>> &getAssetsDB();

    // /**
    //  * @brief 获取资产配置
    //  * @param path 资产路径
    //  * @return std::vector<AssetDB> 资产配置
    //  */
    // const std::vector<AssetDB> &getAssetDB(const std::string &path);
    // /**
    //  * @brief 获取资产配置
    //  * @param uuid 资产uuid
    //  * @return AssetDB 资产配置
    //  */
    // const AssetDB &getAssetDB(const std::string &uuid);

    // // void addAsset(const std::string &uuid, Asset *asset);

    // /**
    //  * @brief 添加资产到缓存
    //  *
    //  * @param uuid 资产uuid
    //  * @param asset 资产指针
    //  */
    // void addAssetByUuid(const std::string &uuid, Asset *asset);

    // // /**
    // //  * @brief 添加资产到缓存
    // //  *
    // //  * @param path 资产路径
    // //  * @param asset 资产指针
    // //  */
    // // void addAsset(const std::string path, Asset *asset);
    // // Asset *getAsset(const std::string &path);

    // // void addSceneAsset(const std::string path, Asset *asset);
    // // Asset *getSceneAsset(const std::string &path);
    ~AssetCache();
};
