#pragma once
#include <vector>

class GfxPushConstants
{
private:
    std::vector<char> _data;
    std::string _key;
    void _addBytes(const void* data, size_t size);
public:
    GfxPushConstants();
    void addInt(const std::string &key, const int value=0);
    void addFloat(const std::string &key, const float value=0.0f);
    void addVec2(const std::string &key, const float x=0.0f, const float y=0.0f);
    void addVec3(const std::string &key, const float x=0.0f, const float y=0.0f, const float z=0.0f);
    void addVec4(const std::string &key, const float x=0.0f, const float y=0.0f, const float z=0.0f, const float w=0.0f);

    /**
     * @brief 获取推送常量的key
     *
     * @return std::string
     */
    std::string getKey() const;
    ~GfxPushConstants();
};
