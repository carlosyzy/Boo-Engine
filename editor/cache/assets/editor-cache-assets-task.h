
#pragma once
#include <string>
#include <filesystem>
#include <functional>
#include "../../../engine/core/utils/json-util.h"
#include "../../../engine/core/assets/asset-struct.h"

class EditorCacheAssetsTask
{
private:
    /**
     * 资产原始路径
     * 全路径
     */
    std::string _originalPath;
    /**
     * 资产库路径
     * 相对路径
     */
    std::string _assetPath;
    /**
     * 资产扩展名
     */
    std::string _assetExtension;
    // /**
    //  * 资产类型
    //  */
    // AssetType _assetType;

    // AssetType getAssetType(const std::string &path);

    // void _parseTextureAssetDB();
    // void _parseMaterialAssetDB();
    // void _parseSceneAssetDB();

    // // json _readAssetBooMapFile(const std::string &path);

    // // void _createANewAssetDB(AssetType assetType);
    // /**
    //  * @brief 更新资产
    //  * 拷贝资产资源到资产库
    //  */
    // void _updateLibraryAsset(json &assetBooMapJson);

public:
    EditorCacheAssetsTask();
    /**
     * 相对路径
     * 父路径
     * 资产配置
     */
    void init(const std::string assetPath);
    void run();
    // /**
    //  * @brief 获取资产路径
    //  * @return std::string 资产路径
    //  */
    std::string getAssetPath();
    ~EditorCacheAssetsTask();
};