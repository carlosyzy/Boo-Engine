#pragma once
#include <vector>
#include <filesystem>

#include "editor-cache-task.h"
#include "../../engine/core/utils/file-util.h"
#include "../../engine/core/utils/json-util.h"

class EditorCache
{
    std::string _settingPath;
    std::string _libraryPath;
    void _initRoot();
    // /**
    //  * @brief 资产配置文件
    //  */
    // json _assetsConfig;
    /**
     * @brief 是否初始化资产映射
     */
    bool _isInitAssetMap = false;
    std::vector<EditorCacheTask> _initAssetMapTasks;
    int _initAssetMapTaskComplete = 0;
    int _initAssetMapTaskAll = 0;
    /**
     * @brief 更新library资产回调
     */
    std::function<void(const int complete, const int all, const float progress)> _initLibraryCallback;

    void _updateLibraryAssetsMaps();

public:
    EditorCache();
    void init();
    template <typename T, typename Func>
    void initLibraryAssetsMaps(Func callback, T *instance)
    {
        this->_initLibraryCallback = [instance, callback](const int complete, const int all, const float progress)
        {
            if (instance != nullptr && callback != nullptr)
            {
                (instance->*callback)(complete, all, progress);
            }
        };
        this->_updateLibraryAssetsMaps();
    }
    /**
     * @brief 更新资产映射
     * 
     * @param path 资产路径
     * @param infos 资产信息
     */
    void updateAssetsMap(std::string path,json infos);


    void update(float deltaTime);

    ~EditorCache();
};
