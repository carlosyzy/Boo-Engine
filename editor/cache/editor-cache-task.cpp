#include "editor-cache-task.h"
#include "../boo-editor.h"

#include "../../engine/boo.h"
#include "../../engine/core/assets/asset.h"
#include "../../engine/core/assets/assets-manager.h"

EditorCacheTask::EditorCacheTask()
{
}
void EditorCacheTask::init(std::string path, json config)
{
    this->_path = path;
    this->_config = config;
}
void EditorCacheTask::run()
{
    if (this->_config.size() > 0)
    {
        bool isOK = true;
        for (auto &config : this->_config)
        {
            // 只要有一个文件不存在就删除旧的重新创建
            std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / config["uuid"]).replace_extension(config["extension"]).generic_string();
            std::string assetPath = (std::filesystem::path(BooEditor::projectPath) / "assets" / config["path"]).generic_string();
            if (!std::filesystem::exists(libraryPath))
            {
                isOK = false;
                break;
            }
            else if (!std::filesystem::exists(assetPath))
            {
                isOK = false;
                break;
            }
        }
        if (!isOK)
        {
            // 删除旧的库资产-然后重新创建
            for (auto &config : this->_config)
            {
                std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / config["uuid"]).replace_extension(config["extension"]).generic_string();
                if (std::filesystem::exists(libraryPath))
                {
                    std::filesystem::remove(libraryPath);
                }
            }
            this->_createLibraryAsset();
        }else{
            // 资产存在-无需重新创建

        }
    }
    else
    {
        // 创建库资产
        this->_createLibraryAsset();
    }
}
void EditorCacheTask::_createLibraryAsset()
{
    // 获取原始路径
    std::filesystem::path assetsPath = (std::filesystem::path(BooEditor::projectPath) / "assets" / this->_path).generic_string();
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
        json configs = json::array();
        std::string uuid = UuidUtil::generateUUID();
        json config = {
            {"type", uint32_t(AssetType::Texture)},
            {"path", this->_path},
            {"uuid", uuid},
            {"extension", extension}};
        configs.push_back(config);
        BooEditor::cache->updateAssetsMap(this->_path, configs);
        // 更新Library资产映射
        std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / uuid).replace_extension(extension).generic_string();
        FileUtil::copyFile(assetsPath.generic_string(), libraryPath);
        Boo::game->assetsManager()->load(libraryPath);
    }
    else if (extension == ".vert" || extension == ".frag") // GLSL-原始资源
    {
        json configs = json::array();
        std::string uuid = UuidUtil::generateUUID();
        json config = {
            {"type", uint32_t(AssetType::Shader)},
            {"path", this->_path},
            {"uuid", uuid},
            {"extension", extension}};
        configs.push_back(config);
        BooEditor::cache->updateAssetsMap(this->_path, configs);
        // 更新Library资产映射
        std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / uuid).replace_extension(extension).generic_string();
        FileUtil::copyFile(assetsPath.generic_string(), libraryPath);
    }
    else if (extension == ".spv") // SPIR-V-原始资源
    {
        json configs = json::array();
        std::string uuid = UuidUtil::generateUUID();
        json config = {
            {"type", uint32_t(AssetType::Shader)},
            {"path", this->_path},
            {"uuid", uuid},
            {"extension", extension}};
        configs.push_back(config);
        BooEditor::cache->updateAssetsMap(this->_path, configs);
        // 更新Library资产映射
        std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / uuid).replace_extension(extension).generic_string();
        FileUtil::copyFile(assetsPath.generic_string(), libraryPath);
    }
    else if (extension == ".scene") // 场景-原始资源
    {
        json configs = json::array();
        std::string uuid = UuidUtil::generateUUID();
        json config = {
            {"type", uint32_t(AssetType::Scene)},
            {"path", this->_path},
            {"uuid", uuid},
            {"extension", extension}};
        configs.push_back(config);
        BooEditor::cache->updateAssetsMap(this->_path, configs);
        // 更新Library资产映射
        std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / uuid).replace_extension(extension).generic_string();
        FileUtil::copyFile(assetsPath.generic_string(), libraryPath);
    }
    else
    {
    }

    // std::string normPath = assetsPath.generic_string();
    // Asset *asset = this->_cache->getAsset(normPath);
    // if (asset != nullptr)
    // {
    //     return;
    // }
    // AssetTask task(this->_mgr, this->_cache, this->_taskID);
    // task.loadAsync(normPath, this->_config);
    // this->_tasks.push_back(task);
}

EditorCacheTask::~EditorCacheTask()
{
}
