#include "editor-cache.h"
#include "../boo-editor.h"

#include "../../engine/boo.h"
#include "../../engine/core/assets/assets-manager.h"
#include "../../engine/core/assets/texture-asset.h"
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
    this->_initAssetsDBTasks.clear();
    // 项目资源db更新
    // std::unordered_map<std::string, std::vector<AssetDB>> &assetsConfig = Boo::game->assetsManager()->getAssetsDB();
    // // 内置资源db更新
    // this->_updateInternalAssetsDBMaps(assetsConfig);
    // // 项目资源db更新
    // this->_updateProjectAssetsDBMaps(assetsConfig);


    // std::cout << "Assets Project: " << this->_assetsPath << std::endl;
    // for (const auto &entry : std::filesystem::recursive_directory_iterator(this->_assetsPath))
    // {
    //     if (std::filesystem::is_regular_file(entry))
    //     {
    //         std::string relativePath = std::filesystem::relative(entry.path(), this->_assetsPath).generic_string();
    //         EditorCacheTask task;
    //         if (assetsConfig.find(relativePath) != assetsConfig.end())
    //         {
    //             task.init(relativePath, this->_assetsPath, assetsConfig[relativePath]);
    //         }
    //         else
    //         {
    //             std::vector<AssetDB> empty;
    //             task.init(relativePath, this->_assetsPath, empty);
    //         }
    //         this->_initAssetsDBTasks.push_back(task);
    //     }
    // }





    this->_isInitAssetsDB = true;
    this->_initAssetsDBTaskAll = this->_initAssetsDBTasks.size();
    this->_initAssetsDBTaskComplete = 0;
    std::cout << "EditorCache::_updateAssetsDBMaps: " << this->_initAssetsDBTaskAll << std::endl;
}
// void EditorCache::_updateInternalAssetsDBMaps(std::unordered_map<std::string, std::vector<AssetDB>> &assetsConfig)
// {
//     std::cout << "Assets Internal: " << this->_internalAssetsPath << std::endl;
//     for (const auto &entry : std::filesystem::recursive_directory_iterator(this->_internalAssetsPath))
//     {
//         if (std::filesystem::is_regular_file(entry))
//         {
//             std::string relativePath = std::filesystem::relative(entry.path(), this->_internalAssetsPath).generic_string();
//             EditorCacheTask task;
//             if (assetsConfig.find(relativePath) != assetsConfig.end())
//             {
//                 task.init(relativePath, this->_internalAssetsPath, assetsConfig[relativePath]);
//             }
//             else
//             {
//                 std::vector<AssetDB> empty;
//                 task.init(relativePath, this->_internalAssetsPath, empty);
//             }
//             this->_initAssetsDBTasks.push_back(task);
//         }
//     }
// }
// void EditorCache::_updateProjectAssetsDBMaps(std::unordered_map<std::string, std::vector<AssetDB>> &assetsConfig)
// {

   
// }

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

void EditorCache::saveAssetsDB()
{
    std::unordered_map<std::string, std::vector<AssetDB>> &assetsConfigs = Boo::game->assetsManager()->getAssetsDB();
    json content = json::object();
    for (auto &assetConfig : assetsConfigs)
    {
        std::string path = assetConfig.first;             // 相对于assets 目录的路径
        std::vector<AssetDB> dbList = assetConfig.second; // 当前目录包含子的子资源
        json subContent = json::array();
         std::cout << "EditorCache::sub: " << path << " " << dbList.size() << std::endl;
        for (auto &db : dbList)
        {
            json subItem = json::object();
            subItem["name"] = db.name;
            subItem["path"] = db.path;
            subItem["uuid"] = db.uuid;
            subItem["type"] = (int)db.type;
            subItem["extension"] = db.extension;
            subContent.push_back(subItem);
           
        }
        content[path] = subContent;
    }
    FileUtil::saveJsonToBinary(this->_assetsDBPath, content);
    std::cout << "EditorCache::saveAssetsDB: " << this->_assetsDBPath << std::endl;
    std::cout << "EditorCache::saveAssetsDB: " << content << std::endl;
}

void EditorCache::addEditorTexture(const std::string &path, TextureAsset *texture)
{
    this->_editorTextures[path] = texture;
}
TextureAsset *EditorCache::getEditorTexture(const std::string &path)
{
    if (this->_editorTextures.find(path) != this->_editorTextures.end())
    {
        return this->_editorTextures[path];
    }
    return nullptr;
}
