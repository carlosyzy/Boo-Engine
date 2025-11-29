#include "asset.h"
#include "../utils/uuid-util.h"

Asset::Asset() {
    this->_uuid = UuidUtil::generateUUID();
}
Asset::Asset(std::string uuid)
{
    this->_uuid = uuid;
}
void Asset::create(std::string path)
{
    this->_path = path;
    //获取资源文件名作为资源名
    this->_name = std::filesystem::path(path).stem().string();
}
void Asset::destroy()
{
}
Asset::~Asset()
{
}
