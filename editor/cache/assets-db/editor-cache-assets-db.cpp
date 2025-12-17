#include "editor-cache-assets-db.h"
#include "../../boo-editor.h"

#include "../../../engine/boo.h"
#include "../../../engine/core/assets/assets-manager.h"
#include "../../../engine/core/assets/texture-asset.h"
#include "../../../engine/core/assets/asset-cache.h"
#include "../../../engine/core/assets/asset-struct.h"
#include "../../../engine/core/utils/file-util.h"
#include "../../../engine/core/utils/json-util.h"
#include "../../../engine/core/utils/uuid-util.h"

EditorCacheAssetsDB::EditorCacheAssetsDB()
{
}
void EditorCacheAssetsDB::init(std::string assetsPath, std::string libraryPath)
{
    // this->_assetsDBPath = (std::filesystem::path(settingPath) / "assets.db").generic_string();
    this->_assetsPath = assetsPath;
    this->_libraryPath = libraryPath;
    // Boo::game->assetsManager()->initAssetsDB(this->_assetsDBPath);
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
            if (!this->_isAssetBooMapFile(entry))
            {
                EditorCacheAssetDBTask task;
                task.init(entry.path().generic_string());
                this->_initAssetsDBTasks.push_back(task);
            }
        }
    }
    this->_initAssetsDBTaskAll = this->_initAssetsDBTasks.size();
    this->_initAssetsDBTaskComplete = 0;
}
bool EditorCacheAssetsDB::_isAssetBooMapFile(std::filesystem::path path)
{
    if (path.extension()!="boo")
    {
        return false;
    }
    else
    {
        // 读取文件,检测内容
        json booConfig = FileUtil::readJsonFromText(path.generic_string());
        if (booConfig.is_null())
        {
            return false;
        }
        if (!booConfig.contains("_type") || !booConfig.contains("uuid"))
        {
            return false;
        }
    }
    return true;
}

void EditorCacheAssetsDB::update(float deltaTime)
{
    if (this->_initAssetsDBTasks.empty())
    {
        return;
    }
    int count = this->_initAssetsDBTasks.size() > 30 ? 30 : this->_initAssetsDBTasks.size();
    for (int i = 0; i < count; i++)
    {
        // 执行任务
        EditorCacheAssetDBTask task = this->_initAssetsDBTasks.front();
        task.run();
        // 任务完成后移除
        this->_initAssetsDBTasks.erase(this->_initAssetsDBTasks.begin());
        // 任务完成后更新进度
        this->_initAssetsDBTaskComplete = this->_initAssetsDBTaskAll - this->_initAssetsDBTasks.size();
        if (this->_progressCallback != nullptr)
        {
            this->_progressCallback(this->_initAssetsDBTaskComplete / (float)this->_initAssetsDBTaskAll, task.getAssetPath());
        }

        if (this->_initAssetsDBTasks.empty() || this->_initAssetsDBTaskComplete >= this->_initAssetsDBTaskAll)
        {
            if (this->_completeCallback != nullptr)
            {
                this->_completeCallback();
            }
            // 清除旧资产库资产
            this->_clearOldLibraryAssets();
            // this->saveAssetsDB();
        }
    }
}
void EditorCacheAssetsDB::_clearOldLibraryAssets()
{
    std::cout << "EditorCacheAssetsDB::_clearOldLibraryAssets" << std::endl;
    AssetCache *assetCache = Boo::game->assetsManager()->getAssetsCache();
    for (const auto &entry : std::filesystem::recursive_directory_iterator(this->_libraryPath))
    {
        if (std::filesystem::is_regular_file(entry))
        {
            std::string assetPath = entry.path().generic_string();
            std::string assetName = std::filesystem::path(assetPath).stem().generic_string();
            if (assetCache->getAssetDB(assetName) == nullptr)
            {
                std::cout << "EditorCacheAssetsDB::_clearOldLibraryAssets: " << assetPath << std::endl;
                std::filesystem::remove(assetPath);
            }
        }
    }
}
// /**
//  * @brief 保存资产数据库
//  *
//  */
// void EditorCacheAssetsDB::saveAssetsDB()
// {
//     // AssetCache *assetCache = Boo::game->assetsManager()->getAssetsCache();
//     // std::unordered_map<std::string, std::vector<AssetDB *>> &pathAssetsDB = assetCache->getPathAssetsDB();
//     // json content = json::object();
//     // for (auto &pathAssetDB : pathAssetsDB)
//     // {
//     //     std::string path = pathAssetDB.first;                  // 相对于assets 目录的路径
//     //     std::vector<AssetDB *> &assetDBs = pathAssetDB.second; // 当前目录包含子的子资源
//     //     json subItems = json::array();
//     //     for (auto &assetDB : assetDBs)
//     //     {
//     //         json subItem = json::object();
//     //         subItem["name"] = assetDB->name;
//     //         subItem["path"] = assetDB->path;
//     //         subItem["uuid"] = assetDB->uuid;
//     //         subItem["type"] = (int)assetDB->type;
//     //         subItem["extension"] = assetDB->extension;
//     //         subItems.push_back(subItem);
//     //     }
//     //     content[path] = subItems;
//     // }
//     // FileUtil::saveJsonToBinary(this->_assetsDBPath, content);
//     // std::cout << "EditorCache::saveAssetsDB: " << this->_assetsDBPath << std::endl;
//     // std::cout << "EditorCache::saveAssetsDB: " << content << std::endl;
// }
EditorCacheAssetsDB::~EditorCacheAssetsDB()
{
}
