#include "editor-cache-task.h"
#include "../boo-editor.h"

#include "../../engine/boo.h"
#include "../../engine/core/assets/asset.h"
#include "../../engine/core/assets/assets-manager.h"

EditorCacheTask::EditorCacheTask()
{
}
void EditorCacheTask::init(std::string path, std::vector<AssetDB> configs)
{
    this->_path = path;
    this->_configs = configs;
}
void EditorCacheTask::run()
{
    std::cout << "EditorCacheTask::run: " << this->_path << " -> " << this->_configs.size() << std::endl;
    if (this->_configs.size() > 0)
    {
        bool isOK = true;
        for (auto &config : this->_configs)
        {
            if (!this->_verifyLibraryAsset(config))
            {
                isOK = false;
                break;
            }
        }
        if (!isOK)
        {
            this->_clearLibraryAsset();
            this->_createLibraryAsset();
        }
        else
        {
            for (auto &config : this->_configs)
            {
                std::string uuid = config.uuid;
                std::string extension = config.extension;
                std::string name = config.name;
                std::string path = config.path;
                std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / uuid).replace_extension(extension).generic_string();
                Boo::game->assetsManager()->load(libraryPath);
            }
        }
    }
    else
    {
        // 创建库资产
        this->_createLibraryAsset();
    }
}
bool EditorCacheTask::_verifyLibraryAsset(AssetDB &config)
{
    std::string uuid = config.uuid;
    std::string extension = config.extension;
    std::string name = config.name;
    std::string path = config.path;

    // 只要有一个文件不存在就删除旧的重新创建
    std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / uuid).replace_extension(extension).generic_string();
    std::string assetPath = (std::filesystem::path(BooEditor::projectPath) / "assets" / path).generic_string();
    if (!std::filesystem::exists(libraryPath))
    {
        return false;
    }
    else if (!std::filesystem::exists(assetPath))
    {
        return false;
    }
    return true;
}
void EditorCacheTask::_clearLibraryAsset()
{
    // 删除旧的库资产-然后重新创建
    for (auto &config : this->_configs)
    {
        std::string uuid = config.uuid;
        std::string extension = config.extension;
        std::string name = config.name;
        std::string path = config.path;
        std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / uuid).replace_extension(extension).generic_string();
        FileUtil::removeFile(libraryPath);
    }
    this->_configs.clear();
}

void EditorCacheTask::_createLibraryAsset()
{
    // 获取原始路径
    std::filesystem::path assetsPath = (std::filesystem::path(BooEditor::projectPath) / "assets" / this->_path).generic_string();
    std::cout << "EditorCacheTask::_createLibraryAsset: " << assetsPath << std::endl;
    if (!std::filesystem::exists(assetsPath))
    {
        return;
    }
    std::string extension = std::filesystem::path(assetsPath).extension().string();
    /**
     * 原始资源类型
     *  1.原始资源-直接拷贝
     *  2.解析资源-解析后存储到库资产中
     */
    if (extension == ".png" || extension == ".PNG" || extension == ".jpg" || extension == ".JPG") // 图片-原始资源
    {
        std::string uuid = UuidUtil::generateUUID();
        std::vector<AssetDB> configs;
        AssetDB sub{};
        sub.name = std::filesystem::path(assetsPath).stem().string();
        sub.path = this->_path;
        sub.uuid = uuid;
        sub.type = AssetType::Texture;
        sub.extension = extension;
        this->_configs.push_back(sub);
        Boo::game->assetsManager()->updateAssetsDB(this->_path, configs);
        // 更新Library资产映射
        std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / uuid).replace_extension(extension).generic_string();
        FileUtil::copyFile(assetsPath.generic_string(), libraryPath);
        Boo::game->assetsManager()->load(libraryPath);
    }
    else if (extension == ".vert" || extension == ".frag") // GLSL-原始资源
    {
        std::string uuid = UuidUtil::generateUUID();
        std::vector<AssetDB> configs;
        AssetDB sub{};
        sub.name = std::filesystem::path(assetsPath).stem().string();
        sub.path = this->_path;
        sub.uuid = uuid;
        sub.type = AssetType::Shader;
        sub.extension = extension;
        this->_configs.push_back(sub);
        Boo::game->assetsManager()->updateAssetsDB(this->_path, configs);
        // 更新Library资产映射
        std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / uuid).replace_extension(extension).generic_string();
        FileUtil::copyFile(assetsPath.generic_string(), libraryPath);
    }
    else if (extension == ".spv") // SPIR-V-原始资源
    {
        std::string uuid = UuidUtil::generateUUID();
        std::vector<AssetDB> configs;
        AssetDB sub{};
        sub.name = std::filesystem::path(assetsPath).stem().string();
        sub.path = this->_path;
        sub.uuid = uuid;
        sub.type = AssetType::Shader;
        sub.extension = extension;
        this->_configs.push_back(sub);
        Boo::game->assetsManager()->updateAssetsDB(this->_path, configs);
        // 更新Library资产映射
        std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / uuid).replace_extension(extension).generic_string();
        FileUtil::copyFile(assetsPath.generic_string(), libraryPath);
    }
    else if (extension == ".scene") // 场景-原始资源
    {
        std::string uuid = UuidUtil::generateUUID();
        std::vector<AssetDB> configs;
        AssetDB sub{};
        sub.name = std::filesystem::path(assetsPath).stem().string();
        sub.path = this->_path;
        sub.uuid = uuid;
        sub.type = AssetType::Scene;
        sub.extension = extension;
        this->_configs.push_back(sub);
        Boo::game->assetsManager()->updateAssetsDB(this->_path, configs);
        // 更新Library资产映射
        std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / uuid).replace_extension(extension).generic_string();
        FileUtil::copyFile(assetsPath.generic_string(), libraryPath);
    }
    else
    {
    }
}

EditorCacheTask::~EditorCacheTask()
{
}
