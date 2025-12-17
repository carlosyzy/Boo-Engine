#pragma once
#include <string>
#include <filesystem>
#include <functional>
#include "../../../engine/core/assets/asset-struct.h"
#include "editor-cache-assets-db-task.h"

class EditorCacheAssetsDB
{
private:
    std::string _assetsPath;
    std::string _libraryPath;
    // std::string _assetsDBPath;

    std::vector<EditorCacheAssetDBTask> _initAssetsDBTasks;
    std::function<void(const float progress, std::string file)> _progressCallback;
    std::function<void()> _completeCallback;
    int _initAssetsDBTaskComplete = 0;
    int _initAssetsDBTaskAll = 0;


    bool _isAssetBooMapFile(std::filesystem::path path);

    /**
     * @brief 清除旧资产库资产
     * 
     */
    void _clearOldLibraryAssets();
public:
    EditorCacheAssetsDB();
    void init(std::string assetsPath, std::string libraryPath);
    void load(std::function<void(const float progress, std::string file)> progress, std::function<void()> complete);
    void update(float deltaTime);
    // /**
    //  * @brief 保存资产数据库
    //  * 
    //  */
    // void saveAssetsDB();
    ~EditorCacheAssetsDB();
};




