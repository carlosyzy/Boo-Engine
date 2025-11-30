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
     * @brief 纹理资产数据库
     * 路径映射资产基本信息
     */
    std::unordered_map<std::string, AssetDB> _textureAssetsDB;
    /**
     * @brief 材质资产数据库
     * 路径映射资产基本信息
     */
    std::unordered_map<std::string, AssetDB> _materialAssetsDB;
    // /**
    //  * @brief 音频资产数据库
    //  * 路径映射资产基本信息
    //  */
    // std::unordered_map<std::string, AssetDB> _audioAssetsDB;
    
    // /**
    //  * @brief 模型资产数据库
    //  */
    // std::unordered_map<std::string, std::vector<AssetDB>> _modelAssetsDB;

    /**
     * @brief 模型资产数据库
     *  uuid--path 映射资产基本信息
     */
    std::unordered_map<std::string, AssetDB> _uuidAssetsDB;

    /**
     * @brief 资产数据库
     *  uuid映射资产指针
     */
    std::unordered_map<std::string, Asset *> _assetsUuidMap;
    // /**
    //  * @brief 资产数据库
    //  *  路径映射资产指针
    //  */
    // std::unordered_map<std::string, Asset *> _assetsPathMap;
    // /**
    //  * @brief 资产数据库
    //  *  场景名称映射资产指针
    //  */
    // std::unordered_map<std::string, Asset *> _sceneAssetsMap;

private:
    /**
     * @brief 解析纹理资产数据库
     * @param path 资产数据库路径
     * @param assetDB 资产数据库
     */
    void _insertTextureAssetDB(const std::string &path, json assetDB);
    /**
     * @brief 解析材质资产数据库
     * @param path 资产数据库路径
     * @param assetDB 资产数据库
     */
    void _insertMaterialAssetDB(const std::string &path, json assetDB);

public:
    AssetCache();
    /**
     * @brief 初始化资产数据库
     * @param path 资产数据库路径
     */
    void initAssetsDB(const std::string &path);
    
    const std::unordered_map<std::string, AssetDB> &_getTextureAssetsDB();
    void _updateTextureAssetsDB(const std::string &path, const AssetDB &config);

    void addAsset(const std::string &uuid, Asset *asset);
    const AssetDB &getAssetDBByUuid(const std::string &uuid);

    
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
    // const AssetDB &getAssetDBByUuid(const std::string &uuid);

    // // void addAsset(const std::string &uuid, Asset *asset);
    Asset *getAssetByUuid(const std::string &uuid);

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
