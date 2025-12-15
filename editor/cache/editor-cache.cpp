#include "editor-cache.h"
#include "../boo-editor.h"
#include "assets-db/editor-cache-assets-db.h"
#include "setting/editor-cache-setting.h"

#include "../../engine/boo.h"
#include "../../engine/core/assets/assets-manager.h"
#include "../../engine/core/assets/texture-asset.h"
#include "../../engine/core/assets/asset-cache.h"

EditorCache::EditorCache()
{
    // 资产目录
    this->_assetsPath = (std::filesystem::path(BooEditor::projectPath) / "assets").generic_string();
    // 判断当前目录存在不,不存在创建
    if (!std::filesystem::exists(this->_assetsPath))
    {
        std::filesystem::create_directories(this->_assetsPath);
    }
    // 配置目录
    this->_settingPath = (std::filesystem::path(BooEditor::projectPath) / "setting").generic_string();
    // 判断当前目录存在不,不存在创建
    if (!std::filesystem::exists(this->_settingPath))
    {
        std::filesystem::create_directories(this->_settingPath);
    }
    // 临时缓存目录
    this->_libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library").generic_string();
    // 判断当前目录存在不,不存在创建
    if (!std::filesystem::exists(this->_libraryPath))
    {
        std::filesystem::create_directories(this->_libraryPath);
    }

    this->_assetsDB = new EditorCacheAssetsDB();
    this->_setting = new EditorCacheSetting();
}
void EditorCache::init()
{
    Boo::game->assetsManager()->setAssetsRoot(this->_libraryPath);
    // 初始化
    this->_assetsDB->init(this->_assetsPath,this->_settingPath, this->_libraryPath);
    this->_setting->init(this->_settingPath);
}
void EditorCache::load(std::function<void(const float progress, std::string file)> progressCallback, std::function<void()> complete)
{
    this->_assetsDB->load(progressCallback, complete);
}
void EditorCache::update(float deltaTime)
{
    this->_assetsDB->update(deltaTime);
    this->_setting->update(deltaTime);
}   

// void EditorCache::_initRoot()
// {
//     // 资产目录
//     std::filesystem::path assetsPath = (std::filesystem::path(BooEditor::projectPath) / "assets").generic_string();
//     // 判断当前目录存在不,不存在创建
//     if (!std::filesystem::exists(assetsPath))
//     {
//         std::filesystem::create_directories(assetsPath);
//     }
//     this->_assetsPath = assetsPath.generic_string();

//     // 配置目录
//
//     // 判断当前目录存在不,不存在创建
//     if (!std::filesystem::exists(settingPath))
//     {
//         std::filesystem::create_directories(settingPath);
//     }
//     this->_settingPath = settingPath.generic_string();

//     // 临时缓存目录
//     std::filesystem::path libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library").generic_string();
//     // 判断当前目录存在不,不存在创建
//     if (!std::filesystem::exists(libraryPath))
//     {
//         std::filesystem::create_directories(libraryPath);
//     }
//     this->_libraryPath = libraryPath.generic_string();

//     // 资产数据库文件
//     std::filesystem::path assetsDBPath = (std::filesystem::path(this->_settingPath) / "assets.db").generic_string();
//     this->_assetsDBPath = assetsDBPath.generic_string();
//     Boo::game->assetsManager()->initAssetsDB(this->_assetsDBPath);
//
// }

// void EditorCache::_initSetting()
// {
//     // 编辑器配置
//     std::string settingPath = this->_settingPath + "/editor.bin";
//     if (std::filesystem::exists(settingPath))
//     {
//         this->_settingEditor = FileUtil::readJsonFromBinary(settingPath);
//     }
//     else
//     {
//         this->_settingEditor = json::object();
//         FileUtil::saveJsonToBinary(settingPath, this->_settingEditor);
//     }
// }

// void EditorCache::_updateAssetsDBMaps()
// {
//     this->_initAssetsDBTasks.clear();
//     // 项目资源db更新
//     for (const auto &entry : std::filesystem::recursive_directory_iterator(this->_assetsPath))
//     {
//         if (std::filesystem::is_regular_file(entry))
//         {
//             EditorCacheTask task;
//             task.init(entry.path().generic_string());
//             this->_initAssetsDBTasks.push_back(task);
//         }
//     }

//     this->_isInitAssetsDB = true;
//     this->_initAssetsDBTaskAll = this->_initAssetsDBTasks.size();
//     this->_initAssetsDBTaskComplete = 0;
//     std::cout << "EditorCache::_updateAssetsDBMaps: " << this->_initAssetsDBTaskAll << std::endl;
// }

// void EditorCache::update(float deltaTime)
// {
//     if (this->_initAssetsDBTasks.size() > 0)
//     {
//         int count = this->_initAssetsDBTasks.size() > 30 ? 30 : this->_initAssetsDBTasks.size();
//         for (int i = 0; i < count; i++)
//         {
//             EditorCacheTask task = this->_initAssetsDBTasks.front();
//             this->_initAssetsDBTasks.erase(this->_initAssetsDBTasks.begin());
//             task.run();
//             this->_initAssetsDBTaskComplete++;
//             if (this->_initAssetsDBCallback != nullptr)
//             {
//                 this->_initAssetsDBCallback(this->_initAssetsDBTaskComplete, this->_initAssetsDBTaskAll, this->_initAssetsDBTaskComplete / (float)this->_initAssetsDBTaskAll);
//             }
//         }
//     }
// }

// void EditorCache::saveAssetsDB()
// {
//     AssetCache *assetCache = Boo::game->assetsManager()->getAssetsCache();
//     const std::unordered_map<std::string, AssetDB> &textureAssetsDB = assetCache->_getTextureAssetsDB();
//     // 保存纹理资产数据库
//     json content = json::object();
//     for (auto &textureAsset : textureAssetsDB)
//     {
//         std::string path = textureAsset.first; // 相对于assets 目录的路径
//         AssetDB db = textureAsset.second;      // 当前目录包含子的子资源
//         json subItem = json::object();
//         subItem["name"] = db.name;
//         subItem["path"] = db.path;
//         subItem["uuid"] = db.uuid;
//         subItem["type"] = (int)db.type;
//         subItem["extension"] = db.extension;
//         content[path] = subItem;
//     }
//     FileUtil::saveJsonToBinary(this->_assetsDBPath, content);
//     std::cout << "EditorCache::saveAssetsDB: " << this->_assetsDBPath << std::endl;
//     std::cout << "EditorCache::saveAssetsDB: " << content << std::endl;
// }

// void EditorCache::addEditorTexture(const std::string &path, TextureAsset *texture)
// {
//     this->_editorTextures[path] = texture;
// }
// TextureAsset *EditorCache::getEditorTexture(const std::string &path)
// {
//     if (this->_editorTextures.find(path) != this->_editorTextures.end())
//     {
//         return this->_editorTextures[path];
//     }
//     return nullptr;
// }
