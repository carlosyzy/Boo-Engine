#include "gfx-push-constants.h"

GfxPushConstants::GfxPushConstants()
{
    this->_data.clear();
}
// 直接添加字节数据（最底层的方法）
void GfxPushConstants::_addBytes(const void *data, size_t size)
{
    size_t oldSize = this->_data.size();
    this->_data.resize(oldSize + size);
    memcpy(this->_data.data() + oldSize, data, size);
}

void GfxPushConstants::addInt(const std::string &key, const int value)
{
    this->_addBytes(&value, sizeof(int));
    this->_key = key + ":" + std::to_string(value) + "|";
}

void GfxPushConstants::addFloat(const std::string &key, const float value)
{
    this->_addBytes(&value, sizeof(float));
    this->_key = key + ":" + std::to_string(value) + "|";
}

void GfxPushConstants::addVec2(const std::string &key, const float x, const float y)
{
    float data[2] = {x, y};
    this->_addBytes(data, 2 * sizeof(float));
    this->_key = key + ":" + std::to_string(x) + ":" + std::to_string(y) + "|";
}

void GfxPushConstants::addVec3(const std::string &key, const float x, const float y, const float z)
{
    float data[3] = {x, y, z};
    this->_addBytes(data, 3 * sizeof(float));
    this->_key = key + ":" + std::to_string(x) + ":" + std::to_string(y) + ":" + std::to_string(z) + "|";
}

void GfxPushConstants::addVec4(const std::string &key, const float x, const float y, const float z, const float w)
{
    float data[4] = {x, y, z, w};
    this->_addBytes(data, 4 * sizeof(float));
    this->_key = key + ":" + std::to_string(x) + ":" + std::to_string(y) + ":" + std::to_string(z) + ":" + std::to_string(w) + "|";
}
/**
 * @brief 获取推送常量的key
 *
 * @return std::string
 */
std::string GfxPushConstants::getKey() const
{
    return this->_key;
}



GfxPushConstants::~GfxPushConstants()
{
}