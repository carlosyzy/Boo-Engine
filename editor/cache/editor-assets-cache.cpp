#include "editor-assets-cache.h"
#include "../boo-editor.h"

EditorAssetsCache::EditorAssetsCache()
{
}
void EditorAssetsCache::init()
{
    std::filesystem::path assetsCachePath = std::filesystem::path(BooEditor::projectPath) / "setting" / "assets.bin";
    if (std::filesystem::is_regular_file(assetsCachePath))
    {
        this->_assetsCache = FileUtil::loadJsonFromBinary(assetsCachePath.string());
    }
    else
    {
        this->_assetsCache = json::object();
    }
    std::cout << "init:Assets Cache: " << this->_assetsCache << std::endl;
    this->reset();
    
}
void EditorAssetsCache::reset()
{
    // 初始化资源列表,和assets下的所有资源进行关联
    std::vector<std::string> existAssets = this->_getAssetsList();
    this->_insertNewAsset(existAssets);
    this->_delNotExistAssets(existAssets);

    std::filesystem::path assetsCachePath = std::filesystem::path(BooEditor::projectPath) / "setting" / "assets.bin";
    std::cout << "update:Assets Cache: " << this->_assetsCache << std::endl;
    FileUtil::saveJsonToBinary(assetsCachePath.string(), this->_assetsCache);
}

void EditorAssetsCache::_insertNewAsset(std::vector<std::string> &existAssets)
{
    // 初始化资源列表,和assets下的所有资源进行关联
    for (const auto &relativePath : existAssets)
    {
        // 如果已经存在,则跳过
        if (this->_assetsCache.contains(relativePath))
        {
            continue;
        }
        else
        {
            this->_assetsCache[relativePath] = json::object();
            // 重新设置数据体
        }
    }
}
void EditorAssetsCache::_delNotExistAssets(std::vector<std::string> &existAssets)
{

    // 待修复
    // 删除不存在的资源
    // 新增临时存储，防止删除异常
    std::vector<std::string> keysToRemove;
    for (const auto& entry : this->_assetsCache.items()) {
        if (std::find(existAssets.begin(), existAssets.end(), entry.key()) == existAssets.end()) {
            keysToRemove.push_back(entry.key());
        }
    }
    for (const auto& key : keysToRemove) {
        this->_assetsCache.erase(key);
    }
    /*for (const auto &entry : this->_assetsCache.items())
    {
        if (std::find(existAssets.begin(), existAssets.end(), entry.key()) == existAssets.end())
        {
            this->_assetsCache.erase(entry.key());
        }
    }*/
}
std::vector<std::string> EditorAssetsCache::_getAssetsList()
{
   // 初始化资源列表,和assets下的所有资源进行关联
    std::vector<std::string> existAssets;
    std::filesystem::path assetsPath = std::filesystem::path(BooEditor::projectPath) / "assets";
    for (const auto &entry : std::filesystem::recursive_directory_iterator(assetsPath))
    {
        if (std::filesystem::is_regular_file(entry))
        {
            std::string relativePath = std::filesystem::relative(entry.path(), std::filesystem::path(BooEditor::projectPath)).string();
            existAssets.push_back(relativePath);
        }
    }
    return existAssets;
}



EditorAssetsCache::~EditorAssetsCache()
{
}
