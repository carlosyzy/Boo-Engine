#include "editor-cache-task.h"
#include "../boo-editor.h"

#include "../../engine/boo.h"
#include "../../engine/core/assets/asset.h"
#include "../../engine/core/assets/assets-manager.h"


EditorCacheTask::EditorCacheTask()
{
}
void EditorCacheTask::init(const std::string originalPath)
{
    this->_originalPath = originalPath;
    this->_assetPath = std::filesystem::relative(this->_originalPath, std::filesystem::path(BooEditor::projectPath) / "assets").generic_string();
    this->_assetExtension = std::filesystem::path(this->_originalPath).extension().generic_string();
    this->_assetType = this->getAssetType(this->_originalPath);
}
void EditorCacheTask::run()
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
void EditorCacheTask::_parseTextureAssetDB()
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
void EditorCacheTask::_parseMaterialAssetDB()
{
}

void EditorCacheTask::_updateLibraryAsset(const AssetDB &db)
{
    std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / db.uuid).replace_extension(db.extension).generic_string();
    FileUtil::copyFile(this->_originalPath, libraryPath);
}
AssetType EditorCacheTask::getAssetType(const std::string &path)
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








// if (this->_configs.size() > 0)
// {
//     bool isOK = true;
//     for (auto &config : this->_configs)
//     {
//         if (!this->_verifyLibraryAsset(config))
//         {
//             isOK = false;
//             break;
//         }
//     }
//     if (!isOK)
//     {
//         this->_clearLibraryAsset();
//         this->_createLibraryAsset();
//     }
//     else
//     {
//         for (auto &config : this->_configs)
//         {
//             std::string uuid = config.uuid;
//             std::string extension = config.extension;
//             std::string name = config.name;
//             std::string path = config.path;
//             // Boo::game->assetsManager()->loadByUuid(uuid);
//         }
//     }
// }
// else
// {
//     // 创建库资产
//     this->_createLibraryAsset();
// }
// }

// bool EditorCacheTask::_verifyLibraryAsset(AssetDB &config)
// {
//     // std::string uuid = config.uuid;
//     // std::string extension = config.extension;
//     // std::string name = config.name;
//     // std::string path = config.path;

//     // // 只要有一个文件不存在就删除旧的重新创建
//     // std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / uuid).replace_extension(extension).generic_string();
//     // std::string originPath = (std::filesystem::path(this->_parentPath) / this->_relativePath).generic_string();
//     // if (!std::filesystem::exists(libraryPath))
//     // {
//     //     std::cout << "File :verifyLibraryAsset:: not exist: " << libraryPath << std::endl;
//     //     return false;
//     // }
//     // else if (!std::filesystem::exists(originPath))
//     // {
//     //     std::cout << "File :verifyLibraryAsset:: not exist: " << originPath << std::endl;
//     //     return false;
//     // }
//     return true;
// }
// void EditorCacheTask::_clearLibraryAsset()
// {
//     // // 删除旧的库资产-然后重新创建
//     // for (auto &config : this->_configs)
//     // {
//     //     std::string uuid = config.uuid;
//     //     std::string extension = config.extension;
//     //     std::string name = config.name;
//     //     std::string path = config.path;
//     //     std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / uuid).replace_extension(extension).generic_string();
//     //     FileUtil::removeFile(libraryPath);
//     // }
//     // this->_configs.clear();
// }

// void EditorCacheTask::_createLibraryAsset()
// {
//     // // 获取原始路径
//     // std::filesystem::path assetsPath = (std::filesystem::path(this->_parentPath) / this->_relativePath).generic_string();
//     // std::cout << "_createLibraryAsset: " << assetsPath << std::endl;
//     // if (!std::filesystem::exists(assetsPath))
//     // {
//     //     return;
//     // }
//     // std::string extension = std::filesystem::path(assetsPath).extension().string();
//     // /**
//     //  * 原始资源类型
//     //  *  1.原始资源-直接拷贝
//     //  *  2.解析资源-解析后存储到库资产中
//     //  */
//     // if (extension == ".png" || extension == ".PNG" || extension == ".jpg" || extension == ".JPG") // 图片-原始资源
//     // {
//     //     std::string uuid = UuidUtil::generateUUID();
//     //     AssetDB sub{};
//     //     sub.name = std::filesystem::path(assetsPath).stem().string();
//     //     sub.path = this->_relativePath;
//     //     sub.uuid = uuid;
//     //     sub.type = AssetType::Texture;
//     //     sub.extension = extension;
//     //     this->_configs.push_back(sub);
//     //     Boo::game->assetsManager()->updateAssetsDB(this->_relativePath, this->_configs);
//     //     // 更新Library资产映射
//     //     std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / uuid).replace_extension(extension).generic_string();
//     //     FileUtil::copyFile(assetsPath.generic_string(), libraryPath);
//     //      Boo::game->assetsManager()->loadByUuid(uuid);
//     // }
//     // else if (extension == ".vert" || extension == ".frag") // GLSL-原始资源
//     // {
//     //     std::string uuid = UuidUtil::generateUUID();
//     //     AssetDB sub{};
//     //     sub.name = std::filesystem::path(assetsPath).stem().string();
//     //     sub.path = this->_relativePath;
//     //     sub.uuid = uuid;
//     //     sub.type = AssetType::Shader;
//     //     sub.extension = extension;
//     //     this->_configs.push_back(sub);
//     //     Boo::game->assetsManager()->updateAssetsDB(this->_relativePath, this->_configs);
//     //     // 更新Library资产映射
//     //     std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / uuid).replace_extension(extension).generic_string();
//     //     FileUtil::copyFile(assetsPath.generic_string(), libraryPath);
//     //      Boo::game->assetsManager()->loadByUuid(uuid);
//     // }
//     // else if (extension == ".spv") // SPIR-V-原始资源
//     // {
//     //     std::string uuid = UuidUtil::generateUUID();
//     //     AssetDB sub{};
//     //     sub.name = std::filesystem::path(assetsPath).stem().string();
//     //     sub.path = this->_relativePath;
//     //     sub.uuid = uuid;
//     //     sub.type = AssetType::Shader;
//     //     sub.extension = extension;
//     //     this->_configs.push_back(sub);
//     //     Boo::game->assetsManager()->updateAssetsDB(this->_relativePath, this->_configs);
//     //     // 更新Library资产映射
//     //     std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / uuid).replace_extension(extension).generic_string();
//     //     FileUtil::copyFile(assetsPath.generic_string(), libraryPath);
//     //      Boo::game->assetsManager()->loadByUuid(uuid);
//     // }
//     // else if (extension == ".scene") // 场景-原始资源
//     // {
//     //     std::string uuid = UuidUtil::generateUUID();
//     //     AssetDB sub{};
//     //     sub.name = std::filesystem::path(assetsPath).stem().string();
//     //     sub.path = this->_relativePath;
//     //     sub.uuid = uuid;
//     //     sub.type = AssetType::Scene;
//     //     sub.extension = extension;
//     //     this->_configs.push_back(sub);
//     //     Boo::game->assetsManager()->updateAssetsDB(this->_relativePath, this->_configs);
//     //     // 更新Library资产映射
//     //     std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / uuid).replace_extension(extension).generic_string();
//     //     FileUtil::copyFile(assetsPath.generic_string(), libraryPath);
//     //      Boo::game->assetsManager()->loadByUuid(uuid);
//     // }
//     // else
//     // {
//     // }
// }

EditorCacheTask::~EditorCacheTask()
{
}
