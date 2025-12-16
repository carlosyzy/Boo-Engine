#include "scene-asset.h"
#include "../utils/file-util.h"
#include "../utils/json-util.h"
#include "../scene/scene.h"

SceneAsset::SceneAsset() : Asset()
{
    this->_type = AssetType::Scene;
}
SceneAsset::SceneAsset(const std::string uuid) : Asset(uuid)
{
    this->_type = AssetType::Scene;
}
void SceneAsset::create(std::string path)
{
    this->_path = path;
    // 获取资源文件名作为资源名
    this->_name = std::filesystem::path(path).stem().string();
    // 加载场景数据
    this->_sceneData = FileUtil::readJsonFromBinary(this->_path);
}
void SceneAsset::_deserializeScene()
{
    // 反序列化场景数据
    // this->_scene = new Scene();
}



SceneAsset::~SceneAsset()
{
}
