#include "asset.h"
Asset::Asset(std::string key) {
    this->_uuid = key;
}
Asset::Asset(std::string key, std::string path)
{
    this->_uuid = key;
    this->_path = path;
    this->_type = AssetType::None;
}
void Asset::destroy()
{
}
Asset::~Asset()
{
}
