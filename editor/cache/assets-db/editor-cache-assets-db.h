#pragma once
#include <string>
#include <filesystem>
#include <functional>
#include "../../../engine/core/assets/asset-struct.h"



class EditorAssetDBTask
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
    /**
     * 资产类型
     */
    AssetType _assetType;

    AssetType getAssetType(const std::string &path);

    void _parseTextureAssetDB();
    void _parseMaterialAssetDB();
    /**
     * @brief 更新资产
     * 拷贝资产资源到资产库
     */
    void _updateLibraryAsset(const AssetDB &db);

public:
    EditorAssetDBTask();
    /**
     * 相对路径
     * 父路径
     * 资产配置
     */
    void init(const std::string assetPath);
    void run();
    /**
     * @brief 获取资产路径
     * @return std::string 资产路径
     */
    std::string getAssetPath();
    ~EditorAssetDBTask();
};




class EditorCacheAssetsDB
{
private:
    std::string _assetsPath;
    std::string _libraryPath;
    std::string _assetsDBPath;

    std::vector<EditorAssetDBTask> _initAssetsDBTasks;
    std::function<void(const float progress, std::string file)> _progressCallback;
    std::function<void()> _completeCallback;
    int _initAssetsDBTaskComplete = 0;
    int _initAssetsDBTaskAll = 0;

public:
    EditorCacheAssetsDB();
    void init(std::string assetsPath, std::string settingPath, std::string libraryPath);
    void load(std::function<void(const float progress, std::string file)> progress, std::function<void()> complete);
    void update(float deltaTime);
    ~EditorCacheAssetsDB();
};




