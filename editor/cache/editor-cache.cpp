#include "editor-cache.h"
#include "../boo-editor.h"

#include "../../engine/boo.h"
#include "../../engine/core/assets/assets-manager.h"
#include "../../engine/core/assets/asset-cache.h"

EditorCache::EditorCache()
{
}
void EditorCache::init()
{
    this->_initRoot();
}
void EditorCache::_initRoot()
{
    // 资产目录
    std::filesystem::path assetsPath = (std::filesystem::path(BooEditor::projectPath) / "assets").generic_string();
    // 判断当前目录存在不,不存在创建
    if (!std::filesystem::exists(assetsPath))
    {
        std::filesystem::create_directories(assetsPath);
    }
    this->_assetsPath = assetsPath.generic_string();

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

    // 资产数据库文件
    std::filesystem::path assetsDBPath = (std::filesystem::path(this->_settingPath) / "assets.db").generic_string();
    this->_assetsDBPath = assetsDBPath.generic_string();
    Boo::game->assetsManager()->initAssetsDB(this->_assetsDBPath);
    Boo::game->assetsManager()->setAssetsRoot(this->_libraryPath);
}

void EditorCache::_updateAssetsDBMaps()
{
    std::cout << "EditorCache::_updateAssetsDBMaps: " << std::endl;
    this->_initAssetsDBTasks.clear();
    std::unordered_map<std::string, std::vector<AssetDB>> &assetsConfig = Boo::game->assetsManager()->getAssetsDB();
    // 获取assets目录下所有资源路径
    for (const auto &entry : std::filesystem::recursive_directory_iterator(this->_assetsPath))
    {
        if (std::filesystem::is_regular_file(entry))
        {
            std::string relativePath = std::filesystem::relative(entry.path(), this->_assetsPath).generic_string();
            EditorCacheTask task;
            if (assetsConfig.find(relativePath) != assetsConfig.end())
            {
                task.init(relativePath, assetsConfig[relativePath]);
            }
            else
            {
                std::vector<AssetDB> empty;
                task.init(relativePath, empty);
            }
            this->_initAssetsDBTasks.push_back(task);
        }
    }
    this->_isInitAssetsDB = true;
    this->_initAssetsDBTaskAll = this->_initAssetsDBTasks.size();
    this->_initAssetsDBTaskComplete = 0;
    std::cout << "EditorCache::_updateAssetsDBMaps: " << this->_initAssetsDBTaskAll << std::endl;
}
void EditorCache::update(float deltaTime)
{
    if (this->_initAssetsDBTasks.size() > 0)
    {
        int count = this->_initAssetsDBTasks.size() > 30 ? 30 : this->_initAssetsDBTasks.size();
        for (int i = 0; i < count; i++)
        {
            EditorCacheTask task = this->_initAssetsDBTasks.front();
            this->_initAssetsDBTasks.erase(this->_initAssetsDBTasks.begin());
            task.run();
            this->_initAssetsDBTaskComplete++;
            if (this->_initAssetsDBCallback != nullptr)
            {
                this->_initAssetsDBCallback(this->_initAssetsDBTaskComplete, this->_initAssetsDBTaskAll, this->_initAssetsDBTaskComplete / (float)this->_initAssetsDBTaskAll);
            }
        }
    }
}