#pragma once
#include <vector>
#include <filesystem>
#include <functional>

#include "../../../engine/core/utils/file-util.h"
#include "../../../engine/core/utils/json-util.h"

class TextureAsset;
class EditorCacheAssets;
class EditorCacheSetting;
class EditorCacheScene;
class Scene;

class EditorCache
{
    std::string _assetsPath;
    std::string _settingPath;
    std::string _libraryPath;

    /**
     * @brief 编辑器资产数据库
     */
    EditorCacheAssets *_assetsCache;
    /**
     * @brief 编辑器配置
     */
    EditorCacheSetting *_settingCache;
    /**
     * @brief 编辑器场景数据库
     */
    EditorCacheScene *_sceneCache;

    void _initAssetsCache();
    void _initSceneCache();

public:
    EditorCache();
    
    EditorCacheScene *SceneCache()
    {
        return this->_sceneCache;
    }
    EditorCacheSetting *SettingCache()
    {
        return this->_settingCache;
    }
    EditorCacheAssets *AssetsCache()
    {
        return this->_assetsCache;
    }

    /**
     * @brief 初始化缓存
     */
    void init();
    /**
     * @brief 加载缓存
     * @param callback 加载进度回调
     */
    void load(std::function<void(const float progress, std::string file)> progressCallback, std::function<void()> complete);
    // /**
    //  * @brief 获取当前场景
    //  * @return std::string 当前场景
    //  */
    // Scene *getCurrentScene();
    // /**
    //  * @brief 保存当前场景
    //  */
    // void saveScene();

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

    // void saveAssetsDB();
};
