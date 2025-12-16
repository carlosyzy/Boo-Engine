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





EditorAssetDBTask::EditorAssetDBTask()
{
}
void EditorAssetDBTask::init(const std::string originalPath)
{
    this->_originalPath = originalPath;
    this->_assetPath = std::filesystem::relative(this->_originalPath, std::filesystem::path(BooEditor::projectPath) / "assets").generic_string();
    this->_assetExtension = std::filesystem::path(this->_originalPath).extension().generic_string();
    this->_assetType = this->getAssetType(this->_originalPath);
}
/**
 * @brief 获取资产路径
 * @return std::string 资产路径
 */
std::string EditorAssetDBTask::getAssetPath()
{
    return this->_assetPath;
}
void EditorAssetDBTask::run()
{
    if (this->_assetType == AssetType::Texture)
    {
        this->_parseTextureAssetDB();
    }
    else if (this->_assetType == AssetType::Material)
    {
        this->_parseMaterialAssetDB();
    }
}
void EditorAssetDBTask::_parseTextureAssetDB()
{
    AssetCache *assetCache = Boo::game->assetsManager()->getAssetsCache();
    const std::unordered_map<std::string, AssetDB> &textureAssetsDB = assetCache->_getTextureAssetsDB();
    if (textureAssetsDB.find(this->_assetPath) == textureAssetsDB.end())
    {
        // 没有找到资产配置
        // 创建资产配置
        // 拷贝资产资源
        std::cout << "EditorCacheTask::create new texture asset db: " << this->_assetPath << std::endl;
        AssetDB newAssetDB{};
        newAssetDB.path = this->_assetPath;
        newAssetDB.extension = this->_assetExtension;
        newAssetDB.uuid = UuidUtil::generateUUID();
        newAssetDB.type = AssetType::Texture;
        newAssetDB.name = std::filesystem::path(this->_assetPath).stem().generic_string();
        assetCache->_updateTextureAssetsDB(this->_assetPath, newAssetDB);
        this->_updateLibraryAsset(newAssetDB);
        Boo::game->assetsManager()->loadByUuid(newAssetDB.uuid);
    }
    else
    {
        // 找到资产
        const AssetDB &assetDB = textureAssetsDB.at(this->_assetPath);
        if (assetDB.uuid.empty() || this->_assetPath != assetDB.path || this->_assetExtension != assetDB.extension)
        {
            // 资产配置为空，或者路径和扩展名不匹配，重新赋值,更新资产配置
            // 拷贝资产资源
            std::cout << "EditorCacheTask::update texture asset db: " << this->_assetPath << std::endl;
            AssetDB newAssetDB{};
            newAssetDB.path = this->_assetPath;
            newAssetDB.extension = this->_assetExtension;
            newAssetDB.uuid = UuidUtil::generateUUID();
            newAssetDB.type = AssetType::Texture;
            newAssetDB.name = std::filesystem::path(this->_assetPath).stem().generic_string();
            assetCache->_updateTextureAssetsDB(this->_assetPath, newAssetDB);
            this->_updateLibraryAsset(newAssetDB);
            Boo::game->assetsManager()->loadByUuid(newAssetDB.uuid);
        }
        else
        {
            // 资产配置OK，无条件重新拷贝资产资源
            this->_updateLibraryAsset(assetDB);
            // 加载资产
            Boo::game->assetsManager()->loadByUuid(assetDB.uuid);
        }
    }
}
void EditorAssetDBTask::_parseMaterialAssetDB()
{
}

void EditorAssetDBTask::_updateLibraryAsset(const AssetDB &db)
{
    std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / db.uuid).replace_extension(db.extension).generic_string();
    FileUtil::copyFile(this->_originalPath, libraryPath);
}
AssetType EditorAssetDBTask::getAssetType(const std::string &path)
{
    std::string extension = std::filesystem::path(path).extension().generic_string();
    if (extension == ".png" || extension == ".PNG" || extension == ".jpg" || extension == ".JPG" || extension == ".jpeg" || extension == ".JPEG")
    {
        return AssetType::Texture;
    }
    else if (extension == ".mtl" || extension == ".Mtl")
    {
        return AssetType::Material;
    }
    else
    {
        return AssetType::None;
    }
    return AssetType::None;
}
EditorAssetDBTask::~EditorAssetDBTask()
{
}







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
    this->_initAssetsDBTaskAll = this->_initAssetsDBTasks.size();
    this->_initAssetsDBTaskComplete = 0;
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
        EditorAssetDBTask task = this->_initAssetsDBTasks.front();
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
        }
    }
}
EditorCacheAssetsDB::~EditorCacheAssetsDB()
{
}
