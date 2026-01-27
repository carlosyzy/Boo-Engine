#include "editor-cache-assets.h"
#include "../../../boo-editor.h"

#include "../../../../engine/boo.h"
#include "../../../../engine/core/assets/assets-manager.h"
#include "../../../../engine/core/assets/texture-asset.h"
#include "../../../../engine/core/assets/asset-cache.h"
#include "../../../../engine/core/assets/asset-struct.h"
#include "../../../../engine/core/utils/file-util.h"
#include "../../../../engine/core/utils/json-util.h"
#include "../../../../engine/core/utils/uuid-util.h"

EditorCacheAssets::EditorCacheAssets()
{
}
void EditorCacheAssets::init(std::string assetsPath)
{
    this->_assetsPath = assetsPath;
}
void EditorCacheAssets::load(std::function<void(const float progress, std::string file)> progress, std::function<void()> complete)
{
    this->_initAssetsTasks.clear();
    this->_progressCallback = progress;
    this->_completeCallback = complete;
    for (const auto &entry : std::filesystem::recursive_directory_iterator(this->_assetsPath))
    {
        if (std::filesystem::is_regular_file(entry))
        {
            EditorCacheAssetsTask task;
            task.init(entry.path().generic_string());
            this->_initAssetsTasks.push_back(task);
        }
    }
    this->_initAssetsTaskAll = this->_initAssetsTasks.size();
    this->_initAssetsTaskComplete = 0;
}

void EditorCacheAssets::update(float deltaTime)
{
    if (this->_initAssetsTasks.empty())
    {
        return;
    }
    int count = this->_initAssetsTasks.size() > 30 ? 30 : this->_initAssetsTasks.size();
    for (int i = 0; i < count; i++)
    {
        // 执行任务
        EditorCacheAssetsTask task = this->_initAssetsTasks.front();
        task.run();
        // 任务完成后移除
        this->_initAssetsTasks.erase(this->_initAssetsTasks.begin());
        // 任务完成后更新进度
        this->_initAssetsTaskComplete = this->_initAssetsTaskAll - this->_initAssetsTasks.size();
        if (this->_progressCallback != nullptr)
        {
            this->_progressCallback(this->_initAssetsTaskComplete / (float)this->_initAssetsTaskAll, task.getAssetPath());
        }

        if (this->_initAssetsTasks.empty() || this->_initAssetsTaskComplete >= this->_initAssetsTaskAll)
        {
            if (this->_completeCallback != nullptr)
            {
                this->_completeCallback();
            }
        }
    }
}
EditorCacheAssets::~EditorCacheAssets()
{
}
