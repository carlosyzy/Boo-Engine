#include "editor-cache.h"
#include "../boo-editor.h"

#include "../../engine/boo.h"
#include "../../engine/core/assets/assets-manager.h"

EditorCache::EditorCache()
{
}
void EditorCache::init()
{
    this->_initRoot();
}
void EditorCache::_initRoot()
{
    // 配置目录
    std::filesystem::path settingPath = (std::filesystem::path(BooEditor::projectPath) / "setting").generic_string();
    // 判断当前目录存在不,不存在创建
    if (!std::filesystem::exists(settingPath))
    {
        std::filesystem::create_directories(settingPath);
    }
    this->_settingPath = settingPath.generic_string();

    // 临时缓存目录
    std::filesystem::path libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library").generic_string();
    // 判断当前目录存在不,不存在创建
    if (!std::filesystem::exists(libraryPath))
    {
        std::filesystem::create_directories(libraryPath);
    }
    this->_libraryPath = libraryPath.generic_string();

    // 资产配置文件
    std::filesystem::path assetsPathMap = (std::filesystem::path(this->_settingPath) / "assets.bin").generic_string();
    Boo::game->assetsManager()->initAssetPathMap(assetsPathMap);
    Boo::game->assetsManager()->setRoot(this->_libraryPath);
}

void EditorCache::_updateLibraryAssetsMaps()
{
    // key : 相对于assets 目录的路径
    // value : 当前目录包含子的子资源
    //        name: 子资源的名称
    //        uuid: 子资源的uuid
    //        type: 子资源的类型
    json &assetsConfig = Boo::game->assetsManager()->getAssetPathMap();
    // 获取assets目录下所有资源路径
    std::filesystem::path assetsPath = (std::filesystem::path(BooEditor::projectPath) / "assets").generic_string();
    // 需要进行映射的资产队列
    this->_initAssetMapTasks.clear();
    for (const auto &entry : std::filesystem::recursive_directory_iterator(assetsPath))
    {
        if (std::filesystem::is_regular_file(entry))
        {
            std::string relativePath = std::filesystem::relative(entry.path(), std::filesystem::path(BooEditor::projectPath)).generic_string();
            EditorCacheTask task;
            if (assetsConfig.contains(relativePath))
            {
                task.init(relativePath, assetsConfig[relativePath]);
            }
            else
            {
                task.init(relativePath, json::array());
            }
            this->_initAssetMapTasks.push_back(task);
        }
    }
    this->_isInitAssetMap = true;
    this->_initAssetMapTaskAll = this->_initAssetMapTasks.size();
    this->_initAssetMapTaskComplete = 0;
}

void EditorCache::updateAssetsMap(std::string path, json infos)
{
    Boo::game->assetsManager()->updateAssetPathMap(path, infos);
}

void EditorCache::update(float deltaTime)
{
    if (this->_initAssetMapTasks.size() > 0)
    {
        int count = this->_initAssetMapTasks.size() > 30 ? 30 : this->_initAssetMapTasks.size();
        for (int i = 0; i < count; i++)
        {
            EditorCacheTask task = this->_initAssetMapTasks.front();
            this->_initAssetMapTasks.erase(this->_initAssetMapTasks.begin());
            task.run();
            this->_initAssetMapTaskComplete++;
            if (this->_initLibraryCallback != nullptr)
            {
                this->_initLibraryCallback(this->_initAssetMapTaskComplete, this->_initAssetMapTaskAll, this->_initAssetMapTaskComplete / (float)this->_initAssetMapTaskAll);
            }
        }
    }
}