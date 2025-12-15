#pragma once
#include <vector>
#include <filesystem>
#include <functional>

#include "editor-cache-task.h"
#include "../../engine/core/utils/file-util.h"
#include "../../engine/core/utils/json-util.h"

class TextureAsset;
class EditorCacheAssetsDB;
class EditorCacheSetting;

class EditorCache
{
    std::string _assetsPath;
    std::string _settingPath;
    std::string _libraryPath;
    
    /**
     * @brief 编辑器资产数据库
     */
    EditorCacheAssetsDB *_assetsDB;
    /**
     * @brief 编辑器配置
     */
    EditorCacheSetting *_setting;



    
    // std::string _assetsDBPath;
    // void _initRoot();
    // /**
    //  * @brief 初始化配置文件
    //  */
    // void _initSetting();
    // json _settingEditor;

    // /**
    //  * @brief 是否初始化资产映射
    //  */
    // bool _isInitAssetsDB = false;
    // std::vector<EditorCacheTask> _initAssetsDBTasks;
    // int _initAssetsDBTaskComplete = 0;
    // int _initAssetsDBTaskAll = 0;
    // /**
    //  * @brief 更新资产数据库回调
    //  */
    // std::function<void(const int complete, const int all, const float progress)> _initAssetsDBCallback;
    // /**
    //  * @brief 更新资产数据库映射
    //  */
    // void _updateAssetsDBMaps();

    // /**
    //  * @brief 编辑器专属纹理资产映射
    //  */
    // std::unordered_map<std::string, TextureAsset *> _editorTextures;

public:
    EditorCache();
    /**
     * @brief 初始化缓存
     */
    void init();
    /**
     * @brief 加载缓存
     * @param callback 加载进度回调
     */
    void load(std::function<void(const float progress,std::string file)> progressCallback, std::function<void()> complete);
     void update(float deltaTime);

    ~EditorCache();


    // template <typename T, typename Func>
    // void initAssetsDBMaps(Func callback, T *instance)
    // {
    //     this->_initAssetsDBCallback = [instance, callback](const int complete, const int all, const float progress)
    //     {
    //         if (instance != nullptr && callback != nullptr)
    //         {
    //             (instance->*callback)(complete, all, progress);
    //         }
    //     };
    //     this->_updateAssetsDBMaps();
    // }

    // void addEditorTexture(const std::string &path, TextureAsset *texture);
    // TextureAsset *getEditorTexture(const std::string &path);

    // void saveAssetsDB();

   
};
