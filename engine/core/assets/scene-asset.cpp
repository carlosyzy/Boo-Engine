#include "scene-asset.h"
#include "../utils/file-util.h"

SceneAsset::SceneAsset(const std::string uuid, const std::string path) : Asset(uuid, path)
{
    this->_type = AssetType::Scene;
    this->_load();
}
void SceneAsset::_load()
{
    this->_sceneData = FileUtil::loadJsonFromBinary(this->_path);
}
SceneAsset::~SceneAsset()
{
}
