#include "editor-cache-assets-db.h"
#include "../../../engine/boo.h"
#include "../../../engine/core/assets/assets-manager.h"
#include "../../../engine/core/assets/texture-asset.h"
#include "../../../engine/core/assets/asset-cache.h"

EditorCacheAssetsDB::EditorCacheAssetsDB()
{
}
void EditorCacheAssetsDB::init(std::string assetsPath, std::string settingPath, std::string libraryPath)
{
    this->_assetsDBPath = (std::filesystem::path(settingPath) / "assets.db").generic_string();
    this->_assetsPath = assetsPath;
    this->_libraryPath = libraryPath;
    Boo::game->assetsManager()->initAssetsDB(this->_assetsDBPath);
}
void EditorCacheAssetsDB::load(std::function<void(const float progress, std::string file)> progress, std::function<void()> complete)
{
    this->_initAssetsDBTasks.clear();
    this->_progressCallback = progress;
    this->_completeCallback = complete;
    for (const auto &entry : std::filesystem::recursive_directory_iterator(this->_assetsPath))
    {
        if (std::filesystem::is_regular_file(entry))
        {
            std::cout << "EditorCacheAssetsDB::load: " << entry.path().generic_string() << std::endl;
            EditorAssetDBTask task;
            task.init(entry.path().generic_string());
            this->_initAssetsDBTasks.push_back(task);
        }
    }
}
void EditorCacheAssetsDB::update(float deltaTime)
{
    // if (this->_initAssetsDBTasks.empty())
    // {
    //     return;
    // }
    // for (auto &task : this->_initAssetsDBTasks)
    // {
    //     if (task.update(deltaTime))
    //     {
    //         this->_progressCallback(task.getProgress(), task.getAssetPath());
    //     }
    // }
    // if (this->_initAssetsDBTasks.size() == this->_initAssetsDBTaskComplete)
    // {
    //     this->_completeCallback();
    // }
}
EditorCacheAssetsDB::~EditorCacheAssetsDB()
{
}
