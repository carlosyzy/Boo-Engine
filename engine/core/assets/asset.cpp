#include "asset.h"
Asset::Asset(std::string key) {
    this->_uuid = key;
}
Asset::Asset(std::string key, std::string path)
{
    this->_uuid = key;
    this->_path = path;
    this->_type = AssetType::None;
    //获取资源文件名作为资源名
    this->_name = std::filesystem::path(key).stem().string();
}
void Asset::destroy()
{
}
Asset::~Asset()
{
}
